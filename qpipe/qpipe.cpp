#include <errno.h>
#include <limits>
#include "qpipe.h"
#include <QDebug>
#include <QDir>
#include <QTemporaryFile>
#include <sys/socket.h>
#include <sys/un.h>  // for unix domain sockets
#include <unistd.h>

QPipe::QPipe(QObject *parent) : QIODevice(parent), _notifier(0),
    _notifierType(QSocketNotifier::Exception), _socket(0)
{
}

QPipe::~QPipe()
{
    if (isOpen()) close();
}

qint64 QPipe::bytesAvailable() const
{
    return _readBuffer.length() + QIODevice::bytesAvailable();
}

void QPipe::close()
{
    QIODevice::close();

    if (0 != _socket) {
        shutdown(_socket, SHUT_RDWR);
        ::close(_socket);
        _socket = 0;
    }

    if (0 != _notifier) {
        _notifier->setEnabled(false);
        delete _notifier;
        _notifier = 0;
    }

    _notifierType = QSocketNotifier::Exception;

    if (QFile::exists(_pipeName) && ! QFile::remove(_pipeName)) {
        qCritical() << "QPipe::close(): Could not remove file "
                    << _pipeName;
    }
}

bool QPipe::connect(QString pipeName)
{
    bool answer = false;
    bool localPipeOpen =  isOpen();

    // TALMAGE: This code originated in protoSocket.cpp:Connect()

    // Create our own pipe to write into if we don't
    // have one already.
    //
    // What happens if uses connect() and listen() on the same file name?
    //
    if (!localPipeOpen) {
        QString fileNameTemplate = QDir::tempPath();
        fileNameTemplate.append("/protoSocketXXXXXX");
        QTemporaryFile tempFile(fileNameTemplate);

        // If we don't turn off the auto remove feature,
        // then when tempFile goes out of scope, its
        // destructor will delete the file. By that time,
        // we will be using it as our own pipe.  Instead,
        // we delete it manually, then let open() create it
        // with the pipe permissions.
        tempFile.setAutoRemove(false);

        if (tempFile.open()) {
            _pipeName = tempFile.fileName();
            tempFile.close();
            tempFile.remove();

            _notifierType = QSocketNotifier::Write;

            if ((localPipeOpen = open(QIODevice::WriteOnly))) {
                QObject::connect(_notifier, SIGNAL(activated(int)),
                                 this, SLOT(writeActivated(int)));
            } else {
                qCritical() << "QPipe::connect(" << pipeName
                            << "): Error opening local domain socket";
            }
        } else {
            qCritical() << "QPipe::connect(" << pipeName
                        << "): Error creating temporary file "
                        << tempFile.fileName();
        }
    }

    //
    // Try to connect to the server, serverPipeName
    //
    if (localPipeOpen) {
        struct sockaddr_un sockAddr;

        // This is how you turn a QString into an array of char.
        // You have to do it in two steps. Otherwise, Qt won't
        // create the QByteArray.
        QByteArray pipeNameByteArray = pipeName.toLocal8Bit();
        const char *pipeNameBytes = pipeNameByteArray.constData();

        int pipeNameBytesLen = strlen(pipeNameBytes);
        int sun_pathMax = sizeof(sockAddr) - SUN_LEN(&sockAddr);
        int len = std::min(sun_pathMax, pipeNameBytesLen);

        if (len == pipeNameBytesLen) {
            memset(&sockAddr, 0, sizeof(sockAddr));
            sockAddr.sun_family = AF_UNIX;
            strncpy(sockAddr.sun_path, pipeNameBytes, len);

            // TALMAGE: omitted some conditional code (from protoSocket.cpp)
            // for calculating len when SCM_RIGHTS is defined. It referenced
            // sockAddr.sun_len which isn't defined on Linux, Maemo, or Meego.
            // Oddly, SCM_RIGHTS is defined on those platforms.

            len += sizeof(sockAddr.sun_family);

            if (answer = (0 == ::connect(_socket, (struct sockaddr*)&sockAddr, len))) {
                qDebug() << "QPipe::connect(): connected to AF_UNIX SOCK_DGRAM " << pipeName;
            } else {
                QString message = tr("QPipe::connect(%1): connect() error %2 %3");
                setErrorString(message.arg(pipeName).arg(errno).arg(strerror(errno)));
                qCritical() << errorString();

                close();
            }
        } else {
            qWarning() << "QPipe::connect(" << pipeName
                       << "): pipe name is too long. It must be no longer than "
                       << sun_pathMax << ".";
            close();
        }
    }

    return answer;
}

bool QPipe::isSequential()
{
    return true;
}

bool QPipe::listen(QString pipeName)
{
    bool answer = false;

    //
    // What happens if uses connect() and listen() on the same file name?
    //

    _notifierType = QSocketNotifier::Read;
    _pipeName = pipeName;

    if (open(QIODevice::ReadOnly)) {
        QObject::connect(_notifier, SIGNAL(activated(int)),
                         this, SLOT(readActivated(int)));
    }

    return answer;
}

bool QPipe::open(OpenMode mode)
{
    bool answer = false;

    // TALMAGE: This code originated in protoSocket.cpp:Open()
    // TALMAGE: Remember! QIODevice subclasses must invoke QIODevice::open()
    if (_notifierType != QSocketNotifier::Exception) {
        if (QIODevice::open(mode)) {
            bool oldPipeOK = true;

            if (QFile::exists(_pipeName)) {
                if (! (oldPipeOK = QFile::remove(_pipeName))) {
                    qCritical() << "QPipe::open(): Could not remove old socket file "
                                << _pipeName;
                    QIODevice::close();
                }
            }

            if (oldPipeOK) {
                struct sockaddr_un sockAddr;

                // This is how you turn a QString into an array of char.
                // You have to do it in two steps. Otherwise, Qt won't
                // create the QByteArray.
                QByteArray pipeNameByteArray = _pipeName.toLocal8Bit();
                const char *pipeNameBytes = pipeNameByteArray.constData();

                int pipeNameBytesLen = strlen(pipeNameBytes);
                int sun_pathMax = sizeof(sockAddr) - SUN_LEN(&sockAddr);
                int len = std::min(sun_pathMax, pipeNameBytesLen);

                if (len == pipeNameBytesLen) {
                    memset(&sockAddr, 0, sizeof(sockAddr));
                    sockAddr.sun_family = AF_UNIX;
                    strncpy(sockAddr.sun_path, pipeNameBytes, len);

                    // TALMAGE: omitted some conditional code (from protoSocket.cpp)
                    // for calculating len when SCM_RIGHTS is defined. It referenced
                    // sockAddr.sun_len which isn't defined on Linux, Maemo, or Meego.
                    // Oddly, SCM_RIGHTS is defined on those platforms.
                    len += sizeof(sockAddr.sun_family);

                    if ((_socket = socket(AF_UNIX, SOCK_DGRAM, 0)) >= 0) {
                        if (answer = (0 == bind(_socket, (struct sockaddr*)&sockAddr,  len))) {
                            _notifier = new QSocketNotifier(_socket, _notifierType);
                            qDebug() << "QPipe::open(): opened AF_UNIX SOCK_DGRAM " << _pipeName;
                        } else {
                            QString message = tr("QPipe::open(): bind(%1) error %2 %3 ");
                            setErrorString(message.arg(_pipeName).arg(errno).arg(strerror(errno)));
                            qCritical() << errorString();

                            close();
                        }
                    } else {
                        QString message = tr("QPipe::open(): socket() error %1 %2");
                        setErrorString(message.arg(errno).arg(strerror(errno)));
                        qCritical() << errorString();

                        close();
                    }
                } else {
                    qWarning() << "QPipe::open(" << _pipeName
                               << "): pipe name is too long. It must be no longer than "
                               << sun_pathMax << ".";
                } // BUG: continuing with truncated pipe name

            } // else: error condition reported and mitigated above
        } else {
            qCritical() << "QPipe()::open(): QIODevice::open(" << mode << ") returned false";
            close();
        }
    } else {
        qCritical() << "QPipe::open(): Don't call open() directly.  Use QPipe::connect() or QPipe::listen() instead.";
    }

    return answer;
}

qint64 QPipe::readData(char *data, qint64 maxlen)
{
    qint64 answer = -1; // No bytes to read.  There will never be more bytes to read.
    QByteArray buf = _readBuffer.left(maxlen);

    char *d = buf.data();
    memcpy(data, d, buf.size());
    _readBuffer.remove(0, buf.size());

    answer = buf.size();
    return answer;
}

void QPipe::readActivated(int socket)
{
    QString message = tr("QPipe::readActivated(): recv() error %1 %2 ");
    _notifier->setEnabled(false);

    // Taken from protoPipe.cpp:Recv()
    int bufferSize = 4*1024; // XXX Magic constant 4K
    char buffer[bufferSize];
    int numRead = 0;

    memset(buffer, 0, bufferSize);
    numRead = recv(socket, buffer, bufferSize, 0);

    if (numRead < 0) {
        switch (errno)
        {
        case EINTR:
        case EAGAIN:
        case ENETRESET:
        case ECONNABORTED:
        case ECONNRESET:
        case ESHUTDOWN:
        case ENOTCONN:
        default:
            setErrorString(message.arg(errno).arg(strerror(errno)));
            qCritical() << errorString();
            break;
        }
    }

    if (numRead > 0) {
        _readBuffer.append(buffer, numRead);
        emit readyRead();
    }

    // The slot connected to readyRead() could close this QPipe.
    // That would set _notifier to zero, causing a SIGSEGV below.
    if (0 != _notifier) {
        _notifier->setEnabled(true);
    }
}

qint64 QPipe::writeData(const char *data, qint64 len)
{
    qint64 answer = -1; // Indicates an error

    answer = send(_socket, data, len, 0);

    if (-1 == answer) {
        QString message = tr("QPipe::writeData(): send() error %1 %2");
        setErrorString(message.arg(errno).arg(strerror(errno)));
        qCritical() << errorString();
    }

    return answer;
}

void QPipe::writeActivated(int socket)
{
    _notifier->setEnabled(false);
    emit readyWrite();

    // The slot connected to readyWrite() could close this QPipe.
    // That would set _notifier to zero, causing a SIGSEGV below.
    if (0 != _notifier) {
        _notifier->setEnabled(true);
    }
}

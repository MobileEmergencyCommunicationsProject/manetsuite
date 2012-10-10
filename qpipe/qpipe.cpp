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

    if (! QFile::remove(_pipeName)) {
        qCritical() << "QPipe::close(): Could not remove file "
                    << _pipeName;
    }
}

bool QPipe::connect(QString serverPipeName)
{
    bool answer = false;
    bool localPipeOpen = true;

    // TALMAGE: This code originated in protoSocket.cpp:Connect()

    _notifierType = QSocketNotifier::Write;

    // Create our own pipe to write into if we don't
    // have one already.
    //
    // What happens if uses connect() and listen() on the same file name?
    //
    if (! isOpen()) {
        QString fileNameTemplate = QDir::tempPath();
        fileNameTemplate.append("protoSocketXXXXXX");
        QTemporaryFile tempFile;

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

            if (open(QIODevice::WriteOnly)) {
                QObject::connect(_notifier, SIGNAL(activated(int)),
                                 this, SLOT(writeActivated(int)));
            } else {
                qCritical() << "QPipe::connect(" << serverPipeName
                            << "): Error opening local domain socket";
                localPipeOpen = false;
            }
        } else {
            qCritical() << "QPipe::connect(" << serverPipeName
                        << "): Error creating temporary file "
                        << tempFile.fileName();
            localPipeOpen = false;
        }
    }

    //
    // Try to connect to the server, serverPipeName
    //
    if (localPipeOpen) {
        struct sockaddr_un sockAddr;
        memset(&sockAddr, 0, sizeof(sockAddr));

        // This is how you turn a QString into an array of char.
        // You have to do it in two steps. Otherwise, Qt won't
        // create the QByteArray.
        QByteArray serverPipeNameByteArray = serverPipeName.toLocal8Bit();
        const char *serverPipeNameBytes = serverPipeNameByteArray.constData();

        int serverPipeNameBytesLen = strlen(serverPipeNameBytes);
        int sun_pathMax = sizeof(sockAddr) - SUN_LEN(&sockAddr);
        int len = std::min(sun_pathMax, serverPipeNameBytesLen);

        if (len != serverPipeNameBytesLen) {
            qWarning() << "QPipe::connect(" << serverPipeName
                       << "): pipe name is too long. It must be shorter than "
                       << sun_pathMax << ".";
        } // BUG: continuing with truncated pipe name

        sockAddr.sun_family = AF_UNIX;
        strncpy(sockAddr.sun_path, serverPipeNameBytes, len);

        int addrLen = strlen(sockAddr.sun_path) + sizeof(sockAddr.sun_family);

        if (::connect(_socket, (struct sockaddr*)&sockAddr, addrLen) == 0) {
            answer = true;
        } else {
            qCritical() << "QPipe::connect(" << serverPipeName
                        << "): connect() error ("
                        << errno << "): " << strerror(errno);
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
    if (QIODevice::open(mode)) {
        if (_notifierType != QSocketNotifier::Exception) {
            struct sockaddr_un sockAddr;
            memset(&sockAddr, 0, sizeof(sockAddr));

            // This is how you turn a QString into an array of char.
            // You have to do it in two steps. Otherwise, Qt won't
            // create the QByteArray.
            QByteArray pipeNameByteArray = _pipeName.toLocal8Bit();
            const char *pipeNameBytes = pipeNameByteArray.constData();

            int pipeNameBytesLen = strlen(pipeNameBytes);
            int sun_pathMax = sizeof(sockAddr) - SUN_LEN(&sockAddr);
            int len = std::min(sun_pathMax, pipeNameBytesLen);

            if (len != pipeNameBytesLen) {
                qWarning() << "QPipe::open(" << _pipeName
                           << "): pipe name is too long. It must be shorter than "
                           << sun_pathMax << ".";
            } // BUG: continuing with truncated pipe name

            sockAddr.sun_family = AF_UNIX;
            strncpy(sockAddr.sun_path, pipeNameBytes, len);

            // TALMAGE: omitted some conditional code (from protoSocket.cpp)
            // for calculating len when SCM_RIGHTS is defined. It referenced
            // sockAddr.sun_len which isn't defined on Linux, Maemo, or Meego.
            // Oddly, SCM_RIGHTS is defined on those platforms.
            len = strlen(sockAddr.sun_path) + sizeof(sockAddr.sun_family);

            if ((_socket = socket(AF_UNIX, SOCK_DGRAM, 0)) >= 0) {
                if (0 == bind(_socket, (struct sockaddr*)&sockAddr,  len)) {
                    _notifier = new QSocketNotifier(_socket, _notifierType);
                    answer = true;
                } else {
                    qCritical() << "QPipe::open() bind(" << _pipeName
                                << ") error (" << errno << "): " << strerror(errno);
                    close();
                }
            } else {
                qCritical() << "QPipe::open() socket() error (" << errno << "): " << strerror(errno);
                close();
            }
        } else {
            qCritical() << "QPipe::open(): Don't call open() directly.  Use QPipe::connect() or QPipe::listen() instead.";
        }
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
    _notifier->setEnabled(false);

    // Taken from protoPipe.cpp:Recv()
    int bufferSize = 4*1024; // XXX Magic constant 4K
    char buffer[bufferSize];
    int numRead = 0;

    memset(buffer, 0, bufferSize);

    numRead = recv(_socket, buffer, bufferSize, 0);

    _readBuffer.append(buffer, numRead);

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
            qCritical() << "QPipe::readActivated(): recv() error: "
                        << errno << ": "
                        << strerror(errno);
            break;
        }
    }

    if (numRead > 0) {
        emit readyRead();
    }

    _notifier->setEnabled(true);
}

qint64 QPipe::writeData(const char *data, qint64 len)
{
    qint64 answer = -1; // Indicates an error

    answer = send(_socket, data, len, 0);

    if (-1 == answer) {
        qCritical() << "QPipe::writeData(): send() error ("
                    << errno << ": "
                    << strerror(errno);
    }

    return answer;
}

void QPipe::writeActivated(int socket)
{
    _notifier->setEnabled(false);
    emit readyWrite();
    _notifier->setEnabled(true);
}

#include <errno.h>
#include "qpipe.h"
#include <QDebug>
#include <sys/socket.h>
#include <sys/un.h>  // for unix domain sockets
#include <unistd.h>

QPipe::QPipe() : QIODevice(), _notifier(0),
    _notifierType(QSocketNotifier::Exception), _socket(0)
{
}

QPipe::QPipe(QObject *parent) : QIODevice(parent)
{
}

QPipe::~QPipe()
{
    if (isOpen()) close();
}

qint64 QPipe::bytesAvailable()
{
    return 0 + QIODevice::bytesAvailable();
}

void QPipe::close()
{
    // TODO: unlink the path

    if (0 != _socket) {
        shutdown(_socket, SHUT_RDWR);
        ::close(_socket);
        _socket = 0;

        if (0 != _notifier) {
            _notifier->setEnabled(false);
            delete _notifier;
            _notifier = 0;
        }
    }
}

bool QPipe::connect(QString pipeName)
{
    bool answer = false;

    _notifierType = QSocketNotifier::Write;
    _pipeName = pipeName;

    if (open(QIODevice::WriteOnly)) {
        QObject::connect(_notifier, SIGNAL(activated(int)),
                     this, SLOT(writeActivated(int)));
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

    // TODO: open the socket.

    if (_notifierType != QSocketNotifier::Exception) {
        QByteArray pipeNameByteArray = _pipeName.toLocal8Bit();

        // This code came from protoSocket.cpp:Open()
        struct sockaddr_un sockAddr;
        memset(&sockAddr, 0, sizeof(sockAddr));
        sockAddr.sun_family = AF_UNIX;
        strcpy(sockAddr.sun_path, pipeNameByteArray.constData());

        // TALMAGE: omitted some conditional code for calculating len
        // TALMAGE: when SCM_RIGHTS is defined. It referenced sockAddr.sun_len
        // TALMAGE: which isn't defined on Linux, Maemo, or Meego. Oddly,
        // TALMAGE: SCM_RIGHTS is defined on those platforms.
        int len = strlen(sockAddr.sun_path) + sizeof(sockAddr.sun_family);

        if (isOpen()) close();

        if ((_socket = socket(AF_UNIX, SOCK_DGRAM, 0)) >= 0) {
            if (0 == bind(handle, (struct sockaddr*)&sockAddr,  len)) {
                _notifier = new QSocketNotifier(_socket, _notifierType);
                answer = true;
            } else {
                qCritical() << "QPipe::open() bind(" << _pipeName
                            << ") error (" << errno << "): " << sterror(errno);
                close();
            }
        } else {
            qCritical() << "QPipe::open() socket() error (" << errno << "): " << sterror(errno);
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
    _notifier->setEnabled(false);

    // Taken from protoPipe.cpp:Recv()
    char buffer[4096]; // XXX Magic constant 4K
    int length = 0;
    int numBytes = 4096;
    int numRead = 0;

    memset(buffer, 0, 4096);

    while ((length = recv(_socket, buffer, numBytes, 0)) > 0) {
           _readBuffer.append(buffer, length);
           numRead += length;
    }

    if (length < 0) {
        numBytes = 0;
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
            qCritical() << "QPipe::readActivated() recv() error: "
                        << errno << ": "
                        << strerror(errno);
            break;
        }
    }

    if (numRead > 0)
        emit readyRead();
}

qint64 QPipe::writeData(const char *data, qint64 len)
{
    qint64 answer = -1; // Indicates an error

    return answer;
}

void QPipe::writeActivated(int socket)
{
    _notifier->setEnabled(false);
    emit readyWrite();
    // XXX Wait until writeData() to enable?
    _notifier->setEnabled(true);
}

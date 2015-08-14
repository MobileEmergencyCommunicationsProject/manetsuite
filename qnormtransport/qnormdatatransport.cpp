#include <QDebug>
#include "qnormdatatransport.h"

QNormDataTransport::QNormDataTransport(const char* address,
                                       unsigned short port,
                                       NormNodeId normNodeId,
                                       QObject *parent) :
    QNormTransport(address, port, normNodeId, parent),
    _stream(&_readBuffer, QIODevice::WriteOnly)
{
}

QNormDataTransport::~QNormDataTransport()
{
}


// Add bytes from a NORM stream to _readBuffer.
// Invoked by NormRxObjectNew() and NormRxObjectUpdated()
void QNormDataTransport::appendToReadBuffer(NormEvent *event)
{
    // TALMAGE: Is there a simpler way to serialize an integer?
    NormObjectHandle handle = event->object;
    NormSize length = NormObjectGetSize(handle);

    const char *data = NormDataAccessData(handle);
    _stream << (qint64)length;
    _stream.writeBytes(data, length);

}

void QNormDataTransport::normRxObjectCompleted(NormEvent *event)
{
    NormObjectType objectType = NormObjectGetType(event->object);

    if (NORM_OBJECT_DATA == objectType) {

        appendToReadBuffer(event);
        emit readyRead();
    } else {

        qCritical() << "QNormDataransport::normRxObjectCompleted(): Received unexpected object: "
                    << QNormTransport::objectTypeToQString(objectType);
        NormObjectCancel(event->object);
    }
}

// Required by QIODevice
qint64 QNormDataTransport::writeData(const char *data, qint64 len)
{
    qint64 answer = 0;

    // TODO!

    return answer;
}

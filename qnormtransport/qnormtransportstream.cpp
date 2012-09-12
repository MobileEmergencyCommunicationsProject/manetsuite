#include <QDebug>
#include "qnormtransportstream.h"

QNormTransportStream::QNormTransportStream(const char *address,
                                           unsigned short port,
                                           NormNodeId normNodeId,
                                           QObject *parent) :
    QNormTransport(address, port, normNodeId, parent),
    _streamHandle(NORM_OBJECT_INVALID)
{
    _senderStreamParameters.bufferSize = QNORMTRANSPORTSTREAM_DEFAULT_BUFFER_SIZE;
    _senderStreamParameters.gracefulShutdown = QNORMTRANSPORTSTREAM_DEFAULT_GRACEFUL_SHUTDOWN;
    _senderStreamParameters.infoLen = 0;
    _senderStreamParameters.infoPtr = 0;
}

QNormTransportStream::~QNormTransportStream()
{
    close();
}

void QNormTransportStream::appendToReadBuffer(NormEvent *event)
{
    // This might not be the best way to read from the NormStream because
    // it might make another copy of data that is already stored in the
    // NORM protocol engine.  On the other hand, once we've read that data,
    // NORM need not keep it around.
    //
    // It would be nice if we could defer all calls to NormStreamRead()
    // until readData() is called.
    //
    unsigned int bytesRead;
    int len = 16*1024; // XXX MAGIC CONSTANT 16K
    char buf[len];

    // Drain the stream.
    // NormStreamRead() indicates end-of-data by setting bytesRead to zero.
    // We can continue reading even when NormStreamRead() returns false.
    //
    while (true) {
        bytesRead = len;
        bool unbrokenStream = NormStreamRead(event->object, buf, &bytesRead);

        if (bytesRead > 0) {
            _readBuffer.append(buf, bytesRead);
        } else break;
    }
}

void QNormTransportStream::close()
{
    if(openMode() && QIODevice::WriteOnly)
        NormStreamClose(_streamHandle, _senderStreamParameters.gracefulShutdown);

    _streamHandle = NORM_OBJECT_INVALID;

    QNormTransport::close();
}

void QNormTransportStream::flush(bool eom, NormFlushMode flushMode)
{
    NormStreamFlush(_streamHandle, eom, flushMode);
}

UINT32 QNormTransportStream::getBufferUsage()
{
    return NormStreamGetBufferUsage(_streamHandle);
}

UINT32 QNormTransportStream::getReadOffset()
{
    return NormStreamGetReadOffset(_streamHandle);
}

bool QNormTransportStream::hasVacancy()
{
    return NormStreamHasVacancy(_streamHandle);
}

void QNormTransportStream::markEom()
{
    NormStreamMarkEom(_streamHandle);
}

void QNormTransportStream::normRxObjectCompleted(NormEvent *event)
{
    NormObjectType objectType = NormObjectGetType(event->object);

//    qDebug() << "QNormTransportStream::normRxObjectCompleted";
    if (NORM_OBJECT_STREAM == objectType) {
        appendToReadBuffer(event);
        emit readyRead();
    } else {
        QString type;

        if (NORM_OBJECT_DATA == objectType) type = "OBJECT";
        else if (NORM_OBJECT_FILE == objectType) type = "FILE";
        else if (NORM_OBJECT_NONE == objectType) type = "INVALID";
        else type = QString("Unknown object type %1").arg(objectType);

        qCritical() << "QNormTransportStream::normRxObjectCompleted(): Received unexpected object: "
                    << type;
    }
}

//
// BUG: Assumes one and only one stream
//
void QNormTransportStream::normRxObjectNew(NormEvent *event)
{
    NormObjectType objectType = NormObjectGetType(event->object);

//    qDebug() << "QNormTransportStream::normRxObjectNew";
    if (NORM_OBJECT_STREAM == objectType) {
        _streamHandle = event->object;
        appendToReadBuffer(event);
        emit readyRead();
    } else {
        QString type;

        if (NORM_OBJECT_DATA == objectType) type = "OBJECT";
        else if (NORM_OBJECT_FILE == objectType) type = "FILE";
        else if (NORM_OBJECT_NONE == objectType) type = "INVALID";
        else type = QString("Unknown object type %1").arg(objectType);

        qCritical() << "QNormTransportStream::normRxObjectNew(): Received unexpected object: "
                    << type;

        NormObjectCancel(event->object);
    }
}

void QNormTransportStream::normRxObjectUpdated(NormEvent *event)
{
    NormObjectType objectType = NormObjectGetType(event->object);

//    qDebug() << "QNormTransportStream::normRxObjectUpdated";
    if (NORM_OBJECT_STREAM == objectType) {
        appendToReadBuffer(event);
        emit readyRead();
    } else {
        //
        // Shouldn't happen because we cancel the other kinds of object in
        // normRxObjectNew().
        //
        QString type;

        if (NORM_OBJECT_DATA == objectType) type = "OBJECT";
        else if (NORM_OBJECT_FILE == objectType) type = "FILE";
        else if (NORM_OBJECT_NONE == objectType) type = "INVALID";
        else type = QString("Unknown object type %1").arg(objectType);

        qCritical() << "QNormTransportStream::normRxObjectUpdated(): Received unexpected object: "
                    << type;
    }
}
void QNormTransportStream::normTxObjectPurged(NormEvent *event)
{
    // emit something
//    qDebug() << "QNormTransportStream::normTxObjectPurged";
}

void QNormTransportStream::normTxObjectSent(NormEvent *event)
{
    // emit something
//    qDebug() << "QNormTransportStream::normTxObjectSent";
}

void QNormTransportStream::normTxQueueEmpty(NormEvent *event)
{
    // emit something to tell the base class that it's possible to write.
//    qDebug() << "QNormTransportStream::normTxQueueEmpty";
    emit readyWrite();
}

void QNormTransportStream::normTxQueueVacancy(NormEvent *event)
{
    // emit something to tell the base class that it's possible to write.
//    qDebug() << "QNormTransportStream::normTxQueueVacancy";
    emit readyWrite();
}

bool QNormTransportStream::open(QIODevice::OpenMode mode)
{
    bool answer = false;

    if (answer = QNormTransport::open(mode)) {
        if (answer && (mode & QIODevice::WriteOnly)) {
            _streamHandle = NormStreamOpen(_normSessionHandle,
                                           _senderStreamParameters.bufferSize,
                                           _senderStreamParameters.infoPtr,
                                           _senderStreamParameters.infoLen);

            if (NORM_OBJECT_INVALID == _streamHandle) {
                qCritical() << "QNormTransportStream::open() NormStreamOpen() returned an invalid value";
            }

            answer = (NORM_OBJECT_INVALID != _streamHandle);
        }
    }

    return answer;
}

bool QNormTransportStream::seekMsgStart()
{
    return NormStreamSeekMsgStart(_streamHandle);
}

void QNormTransportStream::setAutoFlush(NormFlushMode flushMode)
{
    NormStreamSetAutoFlush(_streamHandle, flushMode);
}

void QNormTransportStream::setPushEnable(bool pushEnable)
{
    NormStreamSetPushEnable(_streamHandle, pushEnable);
}

// Writes as much of *data as possible into the stream.
// Sets an end-of-message indication on the stream if
// it was able to write all of *data.
//
qint64 QNormTransportStream::writeData(const char *data, qint64 len)
{
    qint64 answer = 0;

    answer = NormStreamWrite(_streamHandle, data, len);

    return answer;
}

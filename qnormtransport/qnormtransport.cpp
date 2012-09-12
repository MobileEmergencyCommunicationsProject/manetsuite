#include <algorithm>
#include <iostream>
#include "norminstancemanager.h"
#include <QDebug>
#include "qnormtransport.h"

QNormTransport::QNormTransport(const char *address,
                               unsigned short port,
                               NormNodeId normNodeId,
                               QObject *parent) :
    QIODevice(parent),
    _address(address), _normNodeId(normNodeId), _port(port),
    _cacheDirectory(NULL),
    _normSessionIsReceiver(false), _normSessionIsSender(false)
{

    _receiverParameters.bufferSpace = QNORMTRANSPORT_DEFAULT_RECEIVER_BUFFER_SPACE;
    _senderParameters.blockSize = QNORMTRANSPORT_DEFAULT_SENDER_BLOCK_SIZE;
    _senderParameters.bufferSpace = QNORMTRANSPORT_DEFAULT_SENDER_BUFFER_SPACE;
    _senderParameters.numParity = QNORMTRANSPORT_DEFAULT_SENDER_NUM_PARITY;
    _senderParameters.segmentSize = QNORMTRANSPORT_DEFAULT_SENDER_SEGMENT_SIZE;
    _senderParameters.sessionId = QNORMTRANSPORT_DEFAULT_SENDER_SESSION_ID;


    NormInstanceManager *im = NormInstanceManager::instance();
    _normInstanceHandle = im->defaultNormInstanceHandle();
    im->reserveNormInstanceHandle(_normInstanceHandle);
    _normSessionHandle = NormCreateSession(_normInstanceHandle, _address, _port, _normNodeId);
    setUserData(this);
}

QNormTransport::~QNormTransport()
{
    if (QIODevice::NotOpen != openMode())
        close();

    if (NULL != _cacheDirectory) {
        free(_cacheDirectory);
        _cacheDirectory = NULL;
    }

    NormDestroySession(_normSessionHandle);
    NormInstanceManager::instance()->releaseNormInstanceHandle(_normInstanceHandle);
}

qint64 QNormTransport::bytesAvailable()
{
    return _readBuffer.size() + QIODevice::bytesAvailable();
}

const char * QNormTransport::cacheDirectory()
{
    return _cacheDirectory;
}

void QNormTransport::close()
{
    if (openMode() && QIODevice::ReadOnly)
        NormStopReceiver(_normSessionHandle);

    if(openMode() && QIODevice::WriteOnly)
        NormStopSender(_normSessionHandle);

    _normSessionIsReceiver = _normSessionIsSender = false;

    QIODevice::close();
}

NormNodeId QNormTransport::getLocalNodeId()
{
    return NormGetLocalNodeId(_normSessionHandle);
}

const void* QNormTransport::getUserData()
{
    return NormGetUserData(_normSessionHandle);
}

// Return true if this NormTransport is a receiver.
// Return false otherwise.
//
bool QNormTransport::isReceiver()
{
    return _normSessionIsReceiver;
}

// Return true if this NormTransport is a sender.
// Return false otherwise.
//
bool QNormTransport::isSender()
{
    return _normSessionIsSender;
}

void QNormTransport::normCCActive(NormEvent *event)
{
    qDebug() << "QNormTransport::normCCActive";
}

void QNormTransport::normCCInactive(NormEvent *event)
{
    qDebug() << "QNormTransport::normCCInactive";
}

void QNormTransport::normEventInvalid(NormEvent *event)
{
    qDebug() << "QNormTransport::normEventInvalid";
}

void QNormTransport::normGrttUpdated(NormEvent *event)
{
    qDebug() << "QNormTransport::normGrttUpdated";
}

void QNormTransport::normLocalSenderClosed(NormEvent *event)
{
    qDebug() << "QNormTransport::normLocalSenderClosed";
}

void QNormTransport::normRemoteSenderActive(NormEvent *event)
{
    qDebug() << "QNormTransport::normRemoteSenderActive";
}

void QNormTransport::normRemoteSenderInactive(NormEvent *event)
{
    qDebug() << "QNormTransport::normRemoteSenderInactive";
}

void QNormTransport::normRemoteSenderNew(NormEvent *event)
{
    qDebug() << "QNormTransport::normRemoteSenderNew";
}

void QNormTransport::normRemoteSenderPurged(NormEvent *event)
{
    qDebug() << "QNormTransport::normRemoteSenderPurged";
}

void QNormTransport::normRxCmdNew(NormEvent *event)
{
    qDebug() << "QNormTransport::normRxCmdNew";
}

void QNormTransport::normRxObjectAborted(NormEvent *event)
{
    qDebug() << "QNormTransport::normRxObjectAborted";
}

void QNormTransport::normRxObjectCompleted(NormEvent *event)
{
    NormObjectType objectType = NormObjectGetType(event->object);
    QString type;

    if (NORM_OBJECT_DATA == objectType) type = "OBJECT";
    else if (NORM_OBJECT_FILE == objectType) type = "FILE";
    else if (NORM_OBJECT_NONE == objectType) type = "INVALID";
    else type = QString("Unknown object type %1").arg(objectType);

    qDebug() << "QNormTransport::normRxObjectUpdated Completed " << type;
}

void QNormTransport::normRxObjectInfo(NormEvent *event)
{
    qDebug() << "QNormTransport::normRxObjectInfo";
}

void QNormTransport::normRxObjectNew(NormEvent *event)
{
    NormObjectType objectType = NormObjectGetType(event->object);
    QString type;

    if (NORM_OBJECT_DATA == objectType) type = "OBJECT";
    else if (NORM_OBJECT_FILE == objectType) type = "FILE";
    else if (NORM_OBJECT_NONE == objectType) type = "INVALID";
    else type = QString("Unknown object type %1").arg(objectType);

    qDebug() << "QNormTransport::normRxObjectNew New " << type;
}

void QNormTransport::normRxObjectUpdated(NormEvent *event)
{
    NormObjectType objectType = NormObjectGetType(event->object);
    QString type;

    if (NORM_OBJECT_DATA == objectType) type = "OBJECT";
    else if (NORM_OBJECT_FILE == objectType) type = "FILE";
    else if (NORM_OBJECT_NONE == objectType) type = "INVALID";
    else type = QString("Unknown object type %1").arg(objectType);

    qDebug() << "QNormTransport::normRxObjectUpdated " << type;
}

void QNormTransport::normTxCmdSent(NormEvent *event)
{
    qDebug() << "QNormTransport::normTxCmdSent";
}

void QNormTransport::normTxFlushCompleted(NormEvent *event)
{
    qDebug() << "QNormTransport::normTxFlushCompleted";
}

void QNormTransport::normTxObjectPurged(NormEvent *event)
{
    qDebug() << "QNormTransport::normTxObjectPurged";
}

void QNormTransport::normTxObjectSent(NormEvent *event)
{
    qDebug() << "QNormTransport::normTxObjectSent";
}

void QNormTransport::normTxQueueEmpty(NormEvent *event)
{
    qDebug() << "QNormTransport::normTxQueueEmpty";
    emit readyWrite();
}

void QNormTransport::normTxQueueVacancy(NormEvent *event)
{
    qDebug() << "QNormTransport::normTxQueueVacancy";
    emit readyWrite();
}

void QNormTransport::normTxRateChanged(NormEvent *event)
{
    qDebug() << "QNormTransport::normTxRateChanged";
}

void QNormTransport::normTxWatermarkCompleted(NormEvent *event)
{
    qDebug() << "QNormTransport::normTxWatermarkCompleted";
}

bool QNormTransport::open(QIODevice::OpenMode mode)
{
    bool answer = false;
    if (answer = QIODevice::open(mode)) {

        if (mode & QIODevice::ReadOnly) {
            answer = _normSessionIsReceiver =
                    NormStartReceiver(_normSessionHandle,
                                      _receiverParameters.bufferSpace);
            if (!answer) {
                // TODO: set the mode to QIODevice::NotOpen
                qCritical() << "QNormTransport::open() NormStartReceiver() failed";
            } else qDebug() << "Started receiver";
        }

        // BUG: If we could start the receiver but not the
        // sender, we should stop the sender.

        if (answer && (mode & QIODevice::WriteOnly)) {
            answer = _normSessionIsSender =
                    NormStartSender(_normSessionHandle,
                                    _senderParameters.sessionId,
                                    _senderParameters.bufferSpace,
                                    _senderParameters.segmentSize,
                                    _senderParameters.blockSize,
                                    _senderParameters.numParity);
            if (!answer) {
                // TODO: set the mode to QIODevice::NotOpen
                qCritical() << "QNormTransport::open() NormStartSender() failed";
            } qDebug() << "Started sender";
        }
    }

    return answer;
}

qint64 QNormTransport::readData(char *data, qint64 maxlen)
{
   qint64 answer;
   QByteArray buf = _readBuffer.left(maxlen);

   char *d = buf.data();
   memcpy(data, d, buf.size());
   _readBuffer.remove(0, buf.size());

   answer = buf.size();
   return answer;
}

bool QNormTransport::setCacheDirectory(const char* cachePath)
{
    _cacheDirectory = strdup(cachePath);
    // TODO: Ensure that _cacheDirectory ends with a slash

    return NormSetCacheDirectory(_normSessionHandle, cachePath);
}

bool QNormTransport::setFragmentation(bool fragmentation)
{
    return NormSetFragmentation(_normSessionHandle, fragmentation);
}

void QNormTransport::setLoopback(bool loopbackEnable)
{
    NormSetLoopback(_normSessionHandle, loopbackEnable);
}

void QNormTransport::setMulticastInterface(const char* interfaceName)
{
    NormSetMulticastInterface(_normSessionHandle, interfaceName);
}

void QNormTransport::setRxPortReuse(bool enableReuse,
                    const char* rxBindAddress,
                    const char* senderAddress,
                    UINT16 senderPort)
{
    NormSetRxPortReuse(_normSessionHandle, enableReuse,
                       rxBindAddress, senderAddress, senderPort);
}

bool QNormTransport::setTOS(unsigned char tos)
{
    return NormSetTOS(_normSessionHandle, tos);
}

bool QNormTransport::setTTL(unsigned char ttl)
{
    return NormSetTTL(_normSessionHandle, ttl);
}

void QNormTransport::setTxOnly(bool txOnly, bool connectToSessionAddress)
{
    NormSetTxOnly(_normSessionHandle, txOnly, connectToSessionAddress);
}

bool QNormTransport::setTxPort(unsigned short txPort,
               bool enableReuse,
               const char* txBindAddress)
{
    return NormSetTxPort(_normSessionHandle, txPort, enableReuse, txBindAddress);
}

void QNormTransport::setUserData(const void* userData)
{
    NormSetUserData(_normSessionHandle, userData);
}

qint64 QNormTransport::writeData(const char *data, qint64 len)
{
    // TODO: do something useful with h, the NormObjectHandle returned
    // by NormDataEnqueue.

    qint64 answer = -1;
    NormObjectHandle h = NormDataEnqueue(_normSessionHandle, data, len);
    if (NORM_OBJECT_INVALID != h) {
        answer = len;
    }

    return answer;
}

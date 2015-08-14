#ifndef QNORMTRANSPORT_H
#define QNORMTRANSPORT_H

#include <normApi.h>
#include <QByteArray>
#include <QDir>
#include <QIODevice>
#include "qnormtransport_global.h"

typedef struct {
    /*
      See the explanation of NormStartReceiver() in the NORM Developer's Guide.
    */

    /*
      Bytes of buffer space per active NormSender within the NormSession.
    */
    unsigned long bufferSpace;
} NormReceiverParameters;


typedef struct {

    /*
        See the explanation of NormStartSender() in the NORM Developer's Guide
    */

    /*
      The number of source symbol segments (packets) per coding block for
      NORM's systematic Reed-Solomon FEC. This value corresponds to k in
      the traditional systematic block code (n,k) nomenclature.
      */
    unsigned char blockSize;

    /*
      Maximum memory in bytes for buffering any sender calculated FEC segments
      and repair state for the NormSession.
      */
    unsigned long bufferSpace;

    /*
      Maximum number of parity symbol segments (packets) the sender is willing
      to calculate per FEC block.  The parity symbol segments are calculated
      from the blockSize source symbol segments.  This value corresponds to
      n - k in the traditional systematic block code (n,k) nomenclature.
      */
    unsigned char numParity;

    /*
      Maximum payload size in bytes of NORM sender messages. This number does
      not include NORM message header fields.
      */
    unsigned short segmentSize;

    /*
       Uniquely identifies the sender instance within a NormSession.
      */
    NormSessionId sessionId;

} NormSenderParameters;


class QNORMTRANSPORTSHARED_EXPORT QNormTransport : public QIODevice {
    Q_OBJECT

public:
    // Default values for _receiverParameters
    static const unsigned long QNORMTRANSPORT_DEFAULT_RECEIVER_BUFFER_SPACE = 1024 * 1024;

    // Default values for _senderParameters
    static const unsigned char QNORMTRANSPORT_DEFAULT_SENDER_BLOCK_SIZE = 64;
    static const unsigned long QNORMTRANSPORT_DEFAULT_SENDER_BUFFER_SPACE = 1024 * 1024;
    static const unsigned char QNORMTRANSPORT_DEFAULT_SENDER_NUM_PARITY = 16;
    static const unsigned short QNORMTRANSPORT_DEFAULT_SENDER_SEGMENT_SIZE = 1400; //4096;
    static const NormSessionId QNORMTRANSPORT_DEFAULT_SENDER_SESSION_ID = 1;

    //
    // Change these parameters as needed between invoking the
    // constructor and calling open().
    //
    NormReceiverParameters _receiverParameters;
    NormSenderParameters _senderParameters;

    QNormTransport(const char* address, unsigned short port, NormNodeId normNodeId = NORM_NODE_ANY,
                   QObject *parent = 0);
    virtual ~QNormTransport();

    virtual qint64 bytesAvailable();

    const QDir *cacheDirectory();

    virtual void close();

    NormNodeId getLocalNodeId();

    const void* getUserData();

    virtual bool isSequential()
    {
        return true;
    }

    /*
        You can use the NORM API to manipulate the NormSession.
        Remember to use your new powers for good.
      */
    NormSessionHandle normSession() { return _normSessionHandle; }

    //
    // The NormTx* and NormRx* functions correspond to the NormEvent types
    // defined by the NORM API.
    //
    // The events are described in detail in the NORM Developer's Guide.
    //

    /*
    This event indicates that congestion control feedback from receivers has
    begun to be received (This also implies that receivers in the group are
    actually present and can be used as a cue to begin data transmission.).
    Note that congestion control must be enabled (see
    NormSetCongestionControl()) for this event to be posted. Congestion control
    feedback can be assumed to be received until a NORM_CC_INACTIVE event is
    posted.
     */
    virtual void normCCActive(NormEvent *event);

    /*
    This event indicates there has been no recent congestion control feedback
    received from the receiver set and that the local NORM sender has reached
    its minimum transmit rate. Applications may wish to refrain from new data
    transmission until a NORM_CC_ACTIVE event is posted. This notification is
    only posted when congestion control operation is enabled (see
    NormSetCongestionControl()) and a previous NORM_CC_ACTIVE event has
    occurred.
      */
    virtual void normCCInactive(NormEvent *event);

    /*
    This NormEventType indicates an invalid or "null" notification which should
    be ignored.
      */
    virtual void normEventInvalid(NormEvent *event);

    /*
    This notification indicates that either the local sender estimate of GRTT
    has changed, or that a remote sender's estimate of GRTT has changed. The
    "sender" member of the NormEvent is set to NORM_NODE_INVALID if the local
    sender's GRTT estimate has changed or to the NormNodeHandle of the remote
    sender that has updated its estimate of GRTT.
      */
    virtual void normGrttUpdated(NormEvent *event);

    /*
    This event is posted when the NORM protocol engine completes the "graceful
    shutdown" of its participation as a sender in the indicated "session" (see
    NormStopSender()).
      */
    virtual void normLocalSenderClosed(NormEvent *event);

    /*
    This event is posted when a previously inactive (or new) remote sender is
    detected operating as an active sender within the session.
      */
    virtual void normRemoteSenderActive(NormEvent *event);

    /*
    This event is posted after a significant period of inactivity (no sender
    messages received) of a specific NORM sender within the session. The NORM
    protocol engine frees buffering resources allocated for this sender when it
    becomes inactive.
    */
    virtual void normRemoteSenderInactive(NormEvent *event);

    /*
    This event is posted when a receiver first receives messages from a
    specific remote NORM sender. This marks the beginning of the interval
    during which the application may reference the provided "node" handle
    (NormNodeHandle).
    */
    virtual void normRemoteSenderNew(NormEvent *event);

    /*
    This event is posted when the NORM protocol engine frees resources for, and
    thus invalidates the indicated "node" handle.
     */
    virtual void normRemoteSenderPurged(NormEvent *event);

    /*
    This event indicates that an application-defined command has been received
    from a remote sender. The NormEvent node element indicates the
    NormNodeHandle value associated with the given sender. The
    NormNodeGetCommand() call can be used to retrieve the received command
    content.
      */
    virtual void normRxCmdNew(NormEvent *event);

    /*
    This notification is posted when a pending receive object's transmission is
    aborted by the remote sender. Unless the application specifically retains
    the "object" handle, the indicated NormObjectHandle becomes invalid and
    must no longer be referenced.
      */
    virtual void normRxObjectAborted(NormEvent *event);

    /*
    This event is posted when a receive object is completely received,
    including available NORM_INFO content. Unless the application specifically
    retains the "object" handle, the indicated NormObjectHandle becomes invalid
    and must no longer be referenced.
      */
    virtual void normRxObjectCompleted(NormEvent *event);

    /*
    This notification is posted when the NORM_INFO content for the indicated
    "object" is received.
      */
    virtual void normRxObjectInfo(NormEvent *event);

    /*
    This event is posted when reception of a new transport object begins and
    marks the beginning of the interval during which the specified "object"
    (NormObjectHandle) is valid.
      */
    virtual void normRxObjectNew(NormEvent *event);

    /*
    This event indicates that the identified receive "object" has newly
    received data content.
      */
    virtual void normRxObjectUpdated(NormEvent *event);

    /* This event indicates that an application-defined command previously
    enqueued with a call to NormSendCommand() has been transmitted, including
    any repetition.
      */
    virtual void normTxCmdSent(NormEvent *event);

    /*
    This event indicates that the flushing process the NORM sender observes
    when it no longer has data ready for transmission has completed. The
    completion of the flushing process is a reasonable indicator (with a
    sufficient NORM "robust factor" value) that the receiver set no longer has
    any pending repair requests. Note the use of NORM's optional positive
    acknowledgement feature is more deterministic in this regards, but this
    notification is useful when there are non-acking (NACK-only) receivers. The
    default NORM robust factor of 20 (20 flush messages are sent at
    end-of-transmission) provides a high assurance of reliable transmission,
    even with packet loss rates of 50%.
      */
    virtual void normTxFlushCompleted(NormEvent *event);

    /*
    This event indicates that the NORM protocol engine will no longer refer to
    the transport object identified by the event's "object' field. Typically,
    this will occur when the application has enqueued more objects than space
    available within the set sender transmit cache bounds (see
    NormSetTxCacheBounds()). Posting of this notification means the application
    is free to free any resources (memory, files, etc) associated with the
    indicated "object". After this event, the given "object" handle
    (NormObjectHandle) is no longer valid unless it is specifically retained by
    the application's NormObjectHandle.
      */
    virtual void normTxObjectPurged(NormEvent *event);

    /*
    This event indicates that the transport object referenced by the event's
    "object" field has completed at least one pass of total transmission. Note
    that this does not guarantee that reliable transmission has yet completed;
    only that the entire object content has been transmitted. Depending upon
    network behavior, several rounds of NACKing and repair transmissions may be
    required to complete reliable transfer.
      */
    virtual void normTxObjectSent(NormEvent *event);

    /*
    This event indicates the NORM protocol engine has no new data pending
    transmission and the application may enqueue additional objects for
    transmission. If the handle of a sender NORM_OBJECT_STREAM is given in the
    corresponding event "object" field, this indicates the stream transmit
    buffer has been emptied and the sender application may write to the stream
    (Use of NORM_TX_QUEUE_VACANCY may be preferred for this purpose since it
    allows the application to keep the NORM protocol engine busier sending
    data, resulting in higher throughput when attempting very high transfer
    rates).
      */
    virtual void normTxQueueEmpty(NormEvent *event);


    /*
    This event indicates that there is room for additional transmit objects to
    be enqueued, or, if the handle of NORM_OBJECT_STREAM is given in the
    corresponding event "object" field, the application may successfully write
    to the indicated stream object. Note this event is not dispatched until a
    call to NormFileEnqueue(), NormDataEnqueue(), or NormStreamWrite() fails
    because of a filled transmit cache or stream buffer.
      */
    virtual void normTxQueueVacancy(NormEvent *event);

    /*
    This event indicates that NORM Congestion Control operation has adjusted
    the transmission rate. The NormGetTxRate() call may be used to retrieve the
    new corresponding transmission rate. Note that if
    NormSetCongestionControl() was called with its adjustRate parameter set to
    false, then no actual rate change has occurred and the rate value returned
    by NormGetTxRate() reflects a "suggested" rate and not the actual
    transmission rate.
      */
    virtual void normTxRateChanged(NormEvent *event);

    /*
    This event indicates that the flushing process initiated by a prior
    application call to NormSetWatermark() has completed The posting of this
    event indicates the appropriate time for the application to make a call
    NormGetAckingStatus() to determine the results of the watermark flushing
    process.
      */
    virtual void normTxWatermarkCompleted(NormEvent *event);

    static QString objectTypeToQString(NormObjectType type);

    virtual bool open(OpenMode mode);

    bool setCacheDirectory(const QString & cachePath);

    bool setFragmentation(bool fragmentation);

    void setLoopback(bool loopbackEnable);

    void setMulticastInterface(const char* interfaceName);

    void setRxPortReuse(bool enableReuse,
                        const char* rxBindAddress = (const char*)0,
                        const char* senderAddress = (const char*)0,
                        UINT16 senderPort = 0);

    bool setTOS(unsigned char tos);

    bool setTTL(unsigned char ttl);

    void setTxOnly(bool txOnly, bool connectToSessionAddress = false);

    bool setTxPort(unsigned short txPort,
                   bool enableReuse = false,
                   const char* txBindAddress = (const char*)0);

    void setUserData(const void* userData);

signals:
    void readyWrite();

protected:
    // Multicast address or remote node address
    const char *_address;
    NormInstanceHandle _normInstanceHandle;
    NormSessionHandle _normSessionHandle;
    NormNodeId _normNodeId;
    //
    unsigned short _port;

    // In derived classes, NormRxObjectNew() and NormRxObjectUpdated() read
    // bytes from a NORM stream and append them to _readBuffer. In this base
    // class, readData() reads bytes from _readBuffer.
    QByteArray _readBuffer;

    // Add bytes from a NORM stream to _readBuffer.
    // Invoked by NormRxObjectNew() and NormRxObjectUpdated()
    virtual void appendToReadBuffer(NormEvent *event) = 0;

    // Return true if this NormTransport is a receiver.
    // Return false otherwise.
    //
    bool isReceiver();

    // Return true if this NormTransport is a sender.
    // Return false otherwise.
    //
    bool isSender();

    // Required by QIODevice
    qint64 readData(char *data, qint64 maxlen);

    // Required by QIODevice
    //
    // Consider reimplementing writeData().
    // The implememtation in QNormTransport simply calls
    // NormDataEnqueue() and throws away the NormObjectHandle
    // that function returns.
    qint64 writeData(const char *data, qint64 len);

private:
    QDir _cacheDirectory;
    bool _normSessionIsReceiver;
    bool _normSessionIsSender;
};

#endif // QNORMTRANSPORT_H

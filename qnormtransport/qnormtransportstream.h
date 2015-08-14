#ifndef QNORMTRANSPORTSTREAM_H
#define QNORMTRANSPORTSTREAM_H

#include <QNormTransport>

typedef struct {
    /*
      See the NORM Developer's Guide for a description of these stream
      parameters. The documentation for NormStreamOpen() and NormStreamClose()
      are good places to start.
      */

    /*
      Size in bytes of the stream's repair window
      */
    unsigned int bufferSize;

    /*
      Set to true to notify receivers of the shutdown and to
      notify the sender when the shutdown process is complete.
      See NormStreamClose().
      */
    bool gracefulShutdown;

    /*
      An application-specific description of the stream could go here.
      */
    char *infoPtr;

    /*
      The length in bytes of the application-specific description of
      the stream.
      */
    unsigned int infoLen;

} NormStreamParameters;


class QNormTransportStream : public QNormTransport
{
    Q_OBJECT
public:
    /*
      Default values for _senderStreamParameters
      */
    static const unsigned int QNORMTRANSPORTSTREAM_DEFAULT_BUFFER_SIZE = 4 * 1024 * 1024;
    static const bool QNORMTRANSPORTSTREAM_DEFAULT_GRACEFUL_SHUTDOWN = false;
    // The default value for _senderStreamParameters.info is 0.
    // The default value for _senderStreamParameters.infoLen is 0.

    explicit QNormTransportStream(const char* address,
                                  unsigned short port,
                                  NormNodeId normNodeId = NORM_NODE_ANY,
                                  QObject *parent = 0);
    virtual ~QNormTransportStream();

    virtual void close();

    void flush(bool eom = false,
               NormFlushMode flushMode = NORM_FLUSH_PASSIVE);


    UINT32 getBufferUsage();
    UINT32 getReadOffset();

    bool hasVacancy();

    void markEom();

    virtual void normRxObjectCompleted(NormEvent *event);
    virtual void normRxObjectNew(NormEvent *event);
    virtual void normRxObjectUpdated(NormEvent *event);

    virtual void normTxObjectPurged(NormEvent *event);
    virtual void normTxObjectSent(NormEvent *event);
    virtual void normTxQueueEmpty(NormEvent *event);
    virtual void normTxQueueVacancy(NormEvent *event);

    virtual bool open(OpenMode mode);

    bool seekMsgStart();
    void setAutoFlush(NormFlushMode flushMode);
    void setPushEnable(bool pushEnable);

protected:
    NormStreamParameters _senderStreamParameters;
    NormObjectHandle _streamHandle;

    // Add bytes from a NORM stream to _readBuffer in the base class.
    // Invoked by NormRxObjectNew() and NormRxObjectUpdated()
    virtual void appendToReadBuffer(NormEvent *event);

    // Required by QIODevice
    qint64 writeData(const char *data, qint64 len);
};

#endif // QNORMTRANSPORTSTREAM_H

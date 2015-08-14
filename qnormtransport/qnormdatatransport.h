#ifndef QNORMDATATRANSPORT_H
#define QNORMDATATRANSPORT_H

#include <QDataStream>
#include <QNormTransport>
#include <QObject>

class QNormDataTransport : public QNormTransport
{
    Q_OBJECT
public:
    explicit QNormDataTransport(const char* address, unsigned short port, NormNodeId normNodeId = NORM_NODE_ANY,
                                QObject *parent = 0);
    virtual ~QNormDataTransport();

    void normRxObjectCompleted(NormEvent *event);

    //
    // The readyRead signal indicates that a new
    // object is available for reading from the
    // QNormDataTransport.  The object is stored
    // as a <length, value> pair.  The length is
    // a qint64.  The value is an array of bytes.
    //

signals:
    // Raise newObject when this QNormDataTransport
    // recognizes that it is about to receive an object.
    // Receiver can decline to receive the object by
    // calling NormObjectCancel(object);
    //
    void newObject(const NormObjectHandle object);

    // Raise objectQueued when this QNormTransport
    // sucessfully enqueues a file for transmission.
    //
    void objectQueued(const NormObjectHandle object);

    // Raise objectSent when this QNormTransport
    // recognizes that the protocol has completed
    // at least one pass at sending the NormObject.
    // This signal is analogous to the NORM_TX_OBJECT_SENT
    // event of the NORM API.
    //
    // TODO: consider moving objectSent to the base class.
    void objectSent(const NormObjectHandle object);

public slots:
    

protected:
    // Add bytes from a NORM object to _readBuffer.
    // Invoked by NormRxObjectCompleted().
    //
    // The object is written as a <length, value> pair.
    // The length is the number of bytes in the value.
    // It is a qint64.
    //
    // To obtain the object:
    // read the qint64 length
    // read length bytes
    //
    virtual void appendToReadBuffer(NormEvent *event);

    // Required by QIODevice
    qint64 writeData(const char *data, qint64 len);

private:
    // For writing into _readBuffer() in a platform-independent way
    QDataStream _stream;
};

#endif // QNORMDATATRANSPORT_H

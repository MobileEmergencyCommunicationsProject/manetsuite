#ifndef QNORMQFILESTREAM_H
#define QNORMQFILESTREAM_H

#include <QByteArray>
#include <QList>
#include <QNormTransport>

//
// To transmit a file, write() its name as a null-terminated string
// into a QNormFileTransport.  It is up to the user to ensure that
// the file is readable.
//
// At the receiver, the pending arrival of the file is signaled
// with newFile().  The receiver can reject the file by calling
// normObjectCancel().
//
// The completion of the file is signaled by readyRead(). The
// receiver can use read() or readLine() to retrieve the name(s)
// of the received file(s).  Each file name is terminated by a
// newline (\n), so it is natural to use
//
//    QByteArray readLine()
//
class QNormFileTransport : public QNormTransport
{
    Q_OBJECT
public:
    explicit QNormFileTransport(const char* address, unsigned short port, NormNodeId normNodeId = NORM_NODE_ANY,
                              QObject *parent = 0);
    ~QNormFileTransport();
    
    virtual void normRxObjectCompleted(NormEvent *event);
    virtual void normRxObjectInfo(NormEvent *event);
    virtual void normRxObjectNew(NormEvent *event);
    virtual void normRxObjectUpdated(NormEvent *event);

    virtual void normTxObjectSent(NormEvent *event);
    virtual void normTxQueueEmpty(NormEvent *event);
    virtual void normTxQueueVacancy(NormEvent *event);

signals:
    // Raise newFile when this QNormFileTransport
    // recognizes that it is about to receive a file.
    void newFile(const QString & fileName);

public slots:
    
protected:
    // Add bytes from a NORM stream to _readBuffer in the base class.
    // Invoked by NormRxObjectNew() and NormRxObjectUpdated()
    virtual void appendToReadBuffer(NormEvent *event);

    qint64 writeData(const char *data, qint64 len);

private:
    QList<QByteArray> _fileNameQueue; // Names of files waiting to be sent.

    void sendFiles(); // Enqueue for transmission as many file names from _fileNameQueue as will fit.
};

#endif // QNORMQFILESTREAM_H

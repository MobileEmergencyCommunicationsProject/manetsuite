#ifndef QNORMQFILESTREAM_H
#define QNORMQFILESTREAM_H

#include <QByteArray>
#include <QDirIterator>
#include <QList>
#include <QNormTransport>

//
// To transmit a file, write() its name as a newline-terminated string
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
    virtual ~QNormFileTransport();
    
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
    // Receiver can decline to receive the file by
    // calling NormObjectCancel(object);
    //
    void newFile(const QString fileName, const NormObjectHandle object);

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
    // Add bytes from a NORM stream to _readBuffer in the base class.
    // Invoked by NormRxObjectNew() and NormRxObjectUpdated()
    virtual void appendToReadBuffer(NormEvent *event);

    qint64 writeData(const char *data, qint64 len);

private:
    QList<QString> _fileNameQueue; // Names of directories and files waiting to be sent.
    QDirIterator *_directoryIterator;  // Used when sending directories of files.
    QString _currentFile;

    /*
      currentFile(), hasNextFile(), and nextFile()
      implement an iterator over _fileNameQueue and
      _directoryIterator.
    */
    const QString currentFile();
    bool hasNextFile();
    const QString nextFile();

    /* Enqueue for transmission as many file names
       from the _fileNameQueue-_directoryIterator iterator
       as will fit.
    */
    void sendFiles();
};

#endif // QNORMQFILESTREAM_H

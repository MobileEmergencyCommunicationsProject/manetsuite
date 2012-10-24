#ifndef QPIPE_H
#define QPIPE_H

#include <QByteArray>
#include <QIODevice>
#include <QSocketNotifier>

/**
  QPipe is a Qt implementation of the UNIX domain datagram socket.

  Sample usage:

  QPipe toServer;
  toServer.connect("serverName");
  toServer.write("foo");

  QPipe fromClient;
  fromClient.listen("myName");
  QByteArray b = fromClient.readAll();

  Connect to the readyRead() signal to learn when a QPipe is readable.
  */
class QPipe : public QIODevice {
    Q_OBJECT

public:
    explicit QPipe(QObject *parent = 0);

    virtual ~QPipe();

    virtual qint64 bytesAvailable() const;
    virtual void close();

    // Open a pipe for writing.
    // Opening the same pipe for reading and writing is undefined.
    bool connect(QString serverPipeName);

    virtual bool isSequential();

    // Open a pipe for reading
    // Opening the same pipe for reading and writing is undefined.
    bool listen(QString pipeName);

    // Do not call open() directly.
    // Use connect() or listen() instead.
    virtual bool open(OpenMode mode);

protected:
    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);

private slots:
    void readActivated(int socket);

private:
    QSocketNotifier *_notifier;
    QSocketNotifier::Type _notifierType;
    QString _pipeName;

    // When _socket becomes readable, readActivated()
    // copies bytes from it into _readBuffer
    QByteArray _readBuffer;
    int _socket;
};

#endif // QPIPE_H

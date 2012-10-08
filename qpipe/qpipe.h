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

  Connect to the readyRead() or readyWrite() signal to learn
  when a QPipe is readable or writable, respectively.
  */
class QPipe : public QIODevice {
    Q_OBJECT

public:
    QPipe();
    QPipe(QObject *parent);

    virtual ~QPipe();

    virtual qint64 bytesAvailable();
    virtual void close();
    // open a pipe for writing
    bool connect(QString pipeName);
    virtual bool isSequential();
    // open a pipe for reading
    bool listen(QString pipeName);
    virtual bool open(OpenMode mode);

signals:
    void readyWrite();

protected:
    virtual qint64 readData(char *data, qint64 maxlen);
    virtual qint64 writeData(const char *data, qint64 len);

private slots:
    void readActivated(int socket);
    void writeActivated(int socket);

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

#ifndef NORMTEST_H
#define NORMTEST_H

#include <QObject>
#include <QNormTransportStream>
#include <QtDeclarative>
#include <QTimer>
#include <QStringList>

class NormTest : public QObject
{
    Q_OBJECT
public:
    explicit NormTest(QObject *parent = 0);
    virtual ~NormTest();

    Q_INVOKABLE void startReceiver();
    Q_INVOKABLE void startSender();
    Q_INVOKABLE void stop();

signals:
    void newMessage(QString message, QString color);

public slots:
    void on_readyRead();
    void on_readyWrite();
    void update();

private:
    bool _colorValue;
    bool _isReceiver;
    bool _readyWrite;
    qint64 _sequenceNumber;
    QNormTransportStream *_stream;
    QTimer *_timer;

    QString nextColor();
};

#endif // NORMTEST_H

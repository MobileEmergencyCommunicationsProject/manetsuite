#ifndef TESTLOGIC_H
#define TESTLOGIC_H

#include <QObject>
#include <QNormTransport>
#include <QNormTransportStream>
#include <QTimer>

class TestLogic : public QObject
{
    Q_OBJECT
public:
    explicit TestLogic(QNormTransport *receiver = 0,
                       QNormTransportStream *sender = 0,
                       QObject *parent = 0);
    virtual ~TestLogic();

    void enableTimer();

    NormInstanceHandle _instance;
    QNormTransport *_receiver;
    QNormTransportStream *_sender;

//signals:
    
public slots:
    void on_readyRead();
    void on_readyWrite();
    void update();

private:
    bool _readyWrite;
    qint64 _sequenceNumber;
    QTimer *_timer;
};

#endif // TESTLOGIC_H

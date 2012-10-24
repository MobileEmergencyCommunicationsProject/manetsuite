#ifndef SENDER_H
#define SENDER_H

#include "pipetest.h"
#include <QTimer>

class Sender : public PipeTest {
    Q_OBJECT

public:
    explicit Sender(QString pipeName, int timeoutInterval = 1000, QObject *parent = 0);
    virtual ~Sender();

public slots:
    void on_readyRead();
    void on_timeout();

private:
    qint64 _sequenceNumber;
    QTimer *_timer;
    int _timeoutInterval; // Send a message every this-many milliseconds
};
#endif // SENDER_H

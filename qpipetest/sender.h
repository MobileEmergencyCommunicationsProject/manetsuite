#ifndef SENDER_H
#define SENDER_H

#include "pipetest.h"

class Sender : public PipeTest {
    Q_OBJECT

public:
    explicit Sender(QString pipeName, QObject *parent = 0);
    virtual ~Sender();

public slots:
    void on_readyRead();
    void on_readyWrite();

private:
    qint64 _sequenceNumber;
};
#endif // SENDER_H

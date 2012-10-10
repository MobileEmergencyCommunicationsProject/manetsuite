#ifndef RECEIVER_H
#define RECEIVER_H

#include "pipetest.h"

class Receiver : public PipeTest {
    Q_OBJECT

public:
    explicit Receiver(QString pipeName, QObject *parent = 0);
    virtual ~Receiver();

public slots:
    void on_readyRead();
    void on_readyWrite();
};
#endif // RECEIVER_H

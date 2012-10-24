#ifndef PIPETEST_H
#define PIPETEST_H

#include <QObject>
#include <qpipe.h>

class PipeTest : public QObject {
    Q_OBJECT

public:
    explicit PipeTest(QString pipeName, QObject *parent = 0);
    virtual ~PipeTest();

public slots:
    virtual void on_readyRead() = 0;
    virtual void on_timeout() = 0;

protected:
    QPipe _pipe;
    QString _pipeName;
};
#endif // PIPETEST_H

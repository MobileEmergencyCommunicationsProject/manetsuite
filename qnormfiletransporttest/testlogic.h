#ifndef TESTLOGIC_H
#define TESTLOGIC_H

#include <QObject>
#include <QNormFileTransport>

class TestLogic : public QObject
{
    Q_OBJECT
public:
    explicit TestLogic(QNormFileTransport *receiver = 0,
                       QNormFileTransport *sender = 0,
                       QObject *parent = 0);
    virtual ~TestLogic();

    NormInstanceHandle _instance;
    QNormFileTransport *_receiver;
    QNormFileTransport *_sender;

//signals:

public slots:
    void on_newFile(const QString fileName, const NormObjectHandle object);
    void on_objectSent(const NormObjectHandle object);
    void on_readyRead();
    void on_readyWrite();
};

#endif // TESTLOGIC_H

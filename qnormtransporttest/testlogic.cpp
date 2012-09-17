#include <iostream>
#include "testlogic.h"

// Write one message per second.
// Read messages whenever they arrive.
//
// A message is a 64-bit unsigned integer
//

TestLogic::TestLogic(QNormTransport *receiver,
                     QNormTransportStream *sender,
                     QObject *parent) :
    QObject(parent), _receiver(receiver), _sender(sender),
    _readyWrite(false), _sequenceNumber(0), _timer(0)
{

}

TestLogic::~TestLogic()
{
    if (0 != _timer) {
        _timer->stop();
        _timer->deleteLater();
    }

    _receiver = 0;
    _sender = 0;
}

void TestLogic::enableTimer()
{
    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(update()));
    _timer->start(1000);
}

// BUG: The first message always has an extra newline.
void TestLogic::on_readyRead()
{
    int len = 16*1024; // XXX MAGIC CONSTANT 16K
    char buf[len];
    qint64 numRead;
    while ((numRead = _receiver->read(buf, len)) > 0) {
        std::cout.write(buf, numRead);
    }

    // XXX Consider calling _receiver->readAll() once
    // XXX instead of _receiver->read() several times.

    std::cout << std::endl;
}

void TestLogic::on_readyWrite()
{
    _readyWrite = true;
}

// BUG: The first message always has an extra newline.
void TestLogic::update()
{
    qint64 numBytes;

    if (_readyWrite) {
        // BUG: Could write fewer than sizeof(qint64) bytes.
        // The next time on_readyWrite() is called,
        // it should write the remaining bytes.

        QString msg = QString("Message %1").arg(_sequenceNumber);
        QByteArray msgByteArray = msg.toLocal8Bit();
        _sequenceNumber ++;
        numBytes = _sender->write(msgByteArray.constData(), msgByteArray.length() + 1);
        _readyWrite = ((msg.length()+ 1) == numBytes);

        std::cout << msgByteArray.constData() << std::endl;

        if (_readyWrite) {
//            _sender->markEom();
            _sender->flush(true); // mark end-of-message, then flush
        }
    }
}

#include <iostream>
#include <QByteArray>
#include "sender.h"
#include <QDebug>

Sender::Sender(QString pipeName, int timeoutInterval, QObject *parent) :
    PipeTest(pipeName, parent),
    _sequenceNumber(0), _timer(0), _timeoutInterval(timeoutInterval) {
    QObject::connect(&_pipe, SIGNAL(readyRead()),
                     this, SLOT(on_readyRead()));

    _pipe.connect(_pipeName);

    _timer = new QTimer(this);

    QObject::connect(_timer, SIGNAL(timeout()),
                     this, SLOT(on_timeout()));

    _timer->start(_timeoutInterval);
}

Sender::~Sender() {
    if (0 != _timer) {
        _timer->stop();
        delete _timer;
        _timer = 0;
    }
}

void Sender::on_readyRead() {
    QByteArray pipeNameByteArray = _pipeName.toLocal8Bit();
    std::cout << "Sender: Ready to read pipe "
               << pipeNameByteArray.constData()
               << "! CAN'T HAPPEN!"
               << std::endl;
}

void Sender::on_timeout() {
    QString msg = QString("Message %1").arg(_sequenceNumber);
    QByteArray msgByteArray = msg.toLocal8Bit();
    _sequenceNumber ++;

    // number of bytes to write includes the terminating 0
    // in msgByteArray.
    int numBytes = _pipe.write(msgByteArray.constData(),
                               msgByteArray.length() + 1);

    std::cout << msgByteArray.constData() << std::endl;

    if ((msg.length() + 1) != numBytes) {
        std::cout << "Sender: Short write: " << numBytes
                  << " instead of " << msg.length() + 1
                  << std::endl;
    }
}

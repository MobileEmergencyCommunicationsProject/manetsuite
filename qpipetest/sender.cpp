#include <iostream>
#include <QByteArray>
#include "sender.h"

Sender::Sender(QString pipeName, QObject *parent) : PipeTest(pipeName, parent), _sequenceNumber(0) {
    QObject::connect(&pipe, SIGNAL(readyRead()),
                     this, SLOT(on_readyRead()));
    QObject::connect(&pipe, SIGNAL(readyWrite()),
                     this, SLOT(on_readyWrite()));
    pipe.connect(pipeName);
}

Sender::~Sender() {
}

void Sender::on_readyRead() {
    QByteArray pipeNameByteArray = _pipeName.toLocal8Bit();
    std::cout << "Sender: Ready to read pipe "
               << pipeNameByteArray.constData()
               << "! CAN'T HAPPEN!"
               << std::endl;
}

void Sender::on_readyWrite() {
    QString msg = QString("Message %1").arg(_sequenceNumber);
    QByteArray msgByteArray = msg.toLocal8Bit();
    _sequenceNumber ++;

    // number of bytes to write includes the terminating 0
    // in msgByteArra.
    int numBytes = pipe.write(msgByteArray.constData(),
                               msgByteArray.length() + 1);

    std::cout << msgByteArray.constData() << std::endl;

    if ((msg.length() + 1) != numBytes) {
        std::cout << "Sender: Short write: " << numBytes
                  << " instead of " << msg.length() + 1
                  << std::endl;
    }
}

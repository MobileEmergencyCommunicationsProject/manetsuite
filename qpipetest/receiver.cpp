#include <iostream>
#include <QByteArray>
#include "receiver.h"

Receiver::Receiver(QString pipeName, QObject *parent) : PipeTest(pipeName, parent) {
    QObject::connect(&_pipe, SIGNAL(readyRead()),
                     this, SLOT(on_readyRead()));
//    QObject::connect(&_pipe, SIGNAL(readyWrite()),
//                     this, SLOT(on_timeout()));
    _pipe.listen(_pipeName);
}

Receiver::~Receiver() {
}

void Receiver::on_readyRead() {
    qint64 len = 4*1024; // XXX MAGIC CONSTANT 4K
    char buf[len];
    qint64 numRead;

    while ((numRead = _pipe.read(buf, len)) > 0) {
        std::cout.write(buf, numRead);
    }

    // Consider calling _receiver->readAll() once instead of _receiver->read()
    // several times.

    std::cout << std::endl;
}

void Receiver::on_timeout() {
    QByteArray pipeNameByteArray = _pipeName.toLocal8Bit();
    std::cout << "Receiver: Ready to write pipe "
              << pipeNameByteArray.constData()
              << "! CAN'T HAPPEN!"
              << std::endl;
}

#include <iostream>
#include <QtCore/QCoreApplication>
#include <QNormTransport>
#include <QNormTransportStream>
#include <string>
#include "testlogic.h"

#define ADDR "224.1.2.3"
#define INTERFACE "wlan0"
#define PORT 6003

// Return the name of the multicast interface.
// It's INTERFACE, defined above, unless
// there are arguments folling the "-r" or "-s"
// on the command line.  In that case, it's the
// first such argument.
const char *interface(int argc, char *argv[])
{
    char *answer = (char *)INTERFACE;
    if (argc > 2) {
        answer = argv[2];
    }

    std::cout << "interface() returns " << answer << std::endl;
    return (const char *)answer;
}

// Send or receive messages according to the TestLogic
// object.
int main(int argc, char *argv[])
{
    int answer = -1;
    QCoreApplication a(argc, argv);
    TestLogic testLogic;

//    NormSetDebugLevel(10);

    if (argc > 1) {
        if (0 == strcmp("-r", argv[1])) {
            // receiver
            QNormTransport *receiver = new QNormTransportStream(ADDR, PORT, NORM_NODE_ANY);
            QObject::connect(receiver, SIGNAL(readyRead()), &testLogic, SLOT(on_readyRead()));
            receiver->setMulticastInterface(interface(argc, argv));
//            receiver->setRxPortReuse(true);

            testLogic._receiver = receiver;

            if (receiver->open(QIODevice::ReadOnly)) {
                answer = a.exec();
            } else {
                std::cerr << "Could not open the QNormTransport for reading" << std::endl;
            }

        } else if (0 == strcmp("-s", argv[1])) {
            // sender
            QNormTransportStream *sender = new QNormTransportStream(ADDR, PORT, NORM_NODE_ANY);
//            sender->_senderParameters.segmentSize = 24;
            QObject::connect(sender, SIGNAL(readyWrite()), &testLogic, SLOT(on_readyWrite()));
            sender->setMulticastInterface(interface(argc, argv));
//            sender->setRxPortReuse(true);

            testLogic._sender = sender;
            testLogic.enableTimer();

            if (sender->open(QIODevice::WriteOnly)) {
                answer = a.exec();
            } else {
                std::cerr << "Could not open the QNormTransportStream for writing" << std::endl;
            }
        } else {
            std::cout << "Usage: qnormtransporttest -r|-s [interface-name]" << std::endl;
        }
    } else {
        std::cout << "Usage: qnormtransporttest -r|-s [interface-name]" << std::endl;
    }

    return answer;
};


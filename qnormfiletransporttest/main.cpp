#include <iostream>
#include <QDebug>
#include <QDesktopServices>
#include <QtCore/QCoreApplication>
#include <QNormFileTransport>
#include <normObject.h>
#include <string>
#include "testlogic.h"

#define ADDR "224.1.2.3"
#define INTERFACE "wlan0"
#define PORT 6003

const QString receiverDirectoryName() {
    return QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation).append("/").append("NormFileTransportTest");
}

bool createReceiverDirectory()
{
    bool answer = false;
    QString receiverDirName = receiverDirectoryName();
    QDir receiverDir(receiverDirName);

    qDebug() << "Checking or creating " << receiverDirName;

    if (receiverDir.exists() || receiverDir.mkpath(receiverDirName)) {
        answer = true;
    }

    return answer;
}

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

    qDebug() << "interface() returns " << answer;
    return (const char *)answer;
}

void usage() {
    std::cout << "Usage: qnormfiletransporttest -r [interface-name [directory-name]]" << std::endl;
    std::cout << "Usage: qnormfiletransporttest -s interface-name file-name" << std::endl;
}

// Send or receive messages according to the TestLogic
// object.
int main(int argc, char *argv[])
{
    int answer = -1;
    QCoreApplication a(argc, argv);
    TestLogic testLogic;

//    NormSetDebugLevel(4);

    if (argc > 1) {
        if (0 == strcmp("-r", argv[1])) {
            // receiver
            QNormFileTransport *receiver = new QNormFileTransport(ADDR, PORT, NORM_NODE_ANY);

            QObject::connect(receiver, SIGNAL(readyRead()),
                             &testLogic, SLOT(on_readyRead()));

            QObject::connect(receiver, SIGNAL(newFile(QString,NormObjectHandle)),
                             &testLogic, SLOT(on_newFile(QString,NormObjectHandle)));

            receiver->setMulticastInterface(interface(argc, argv));
//            receiver->setRxPortReuse(true);

            testLogic._receiver = receiver;

            if (createReceiverDirectory()) {
                receiver->setCacheDirectory(receiverDirectoryName());

                if (receiver->open(QIODevice::ReadOnly)) {
                    answer = a.exec();
                } else {
                    qCritical() << "Could not open the QNormFileTransport for reading";
                }
            } else {
                qCritical() << "Could not create the receiver directory";
            }
        } else if ((0 == strcmp("-s", argv[1])) && (argc > 3)) {// programname -s wlan0 file file file ...
            // sender
            NormFileList files;
            QNormFileTransport *sender = new QNormFileTransport(ADDR, PORT, NORM_NODE_ANY);

            QObject::connect(sender, SIGNAL(objectSent(NormObjectHandle)),
                             &testLogic, SLOT(on_objectSent(NormObjectHandle)));

            QObject::connect(sender, SIGNAL(readyWrite()),
                             &testLogic, SLOT(on_readyWrite()));

            sender->setMulticastInterface(interface(argc, argv));
            //            sender->setRxPortReuse(true);

            testLogic._sender = sender;

            if (sender->open(QIODevice::WriteOnly)) {
                for (int i=3; i < argc; i++) {
                    QString *fileName = new QString(argv[i]);

                    //
                    // Remember! Each line we write into a QNormFileTransport
                    // is the name of a file that we want NORM to send.
                    //
                    fileName->append("\n");
                    QByteArray fileNameByteArray = fileName->toLocal8Bit();
                    sender->write(fileNameByteArray.constData(), fileNameByteArray.size());

                    delete fileName;
                }

                answer = a.exec();
            } else {
                qCritical() << "Could not open the QNormFileTransportStream for writing";
            }
        } else {
            usage();
        }
    } else {
        usage();
    }

    return answer;
};

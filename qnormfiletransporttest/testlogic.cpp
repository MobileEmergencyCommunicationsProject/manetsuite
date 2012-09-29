#include <protoDefs.h> // Defines PATH_MAX
#include <iostream>
#include <QDebug>
#include "testlogic.h"

// Write one message per second.
// Read messages whenever they arrive.
//
// A message is a 64-bit unsigned integer
//

TestLogic::TestLogic(QNormFileTransport *receiver,
                     QNormFileTransport *sender,
                     QObject *parent) :
    QObject(parent), _receiver(receiver), _sender(sender)
{

}

TestLogic::~TestLogic()
{
    _receiver = 0;
    _sender = 0;
}

void TestLogic::on_newFile(const QString fileName, const NormObjectHandle object)
{
    qDebug() << "New file: " << fileName;
}

void TestLogic::on_objectSent(const NormObjectHandle object)
{
    char fileName[PATH_MAX];

    // BUG: assumes that object is a NormFile
    if (NormFileGetName(object, fileName, PATH_MAX)) {
        //
        // It's safe to print fileName because NormFileGetName()
        // ensures that it is zero-terminated.
        //
        qDebug() << "Sent file " << fileName;
    } else {
        qDebug() << "on_objectSent(): SHOULDN'T HAPPEN! Sent file but its name isn't valid.  This is a bug.";
    }
}

// BUG: The first message always has an extra newline.
void TestLogic::on_readyRead()
{
    QByteArray fileName = _receiver->readLine();

    while (fileName.size() > 0) {
        qDebug() << "Received file " << fileName;
        fileName = _receiver->readLine();
    }
}

void TestLogic::on_readyWrite()
{
    // do something
    qDebug() << "Ready to write";

}


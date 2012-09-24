#include "normfiletest.h"
#include <QDebug>
#include <QDesktopServices>

#define ADDR "224.1.2.3"
#define INTERFACE "wlan0"
#define PORT 6003

QVariant RECEIVER("receiver");
QVariant SENDER("sender");

// TODO: Subclass NormFileTransport and override
//       virtual void normRxObjectInfo(NormEvent *event);
//
//       so that we can report file name changes.

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


NormFileTest::NormFileTest(QObject *parent) :
    QObject(parent)
{
    _transport = new QNormFileTransport(ADDR, PORT, NORM_NODE_ANY);

    //
    // receiver signals & slots
    //
    QObject::connect(_transport, SIGNAL(readyRead()),
                     this, SLOT(on_readyRead()));

    QObject::connect(_transport, SIGNAL(newFile(QString,NormObjectHandle)),
                     this, SLOT(on_newFile(QString,NormObjectHandle)));

    //
    // sender signals & slots
    //
    QObject::connect(_transport, SIGNAL(objectSent(NormObjectHandle)),
                     this, SLOT(on_objectSent(NormObjectHandle)));

    QObject::connect(_transport, SIGNAL(readyWrite()),
                     this, SLOT(on_readyWrite()));

    _transport->setMulticastInterface("wlan0");
//            receiver->setRxPortReuse(true);


    if (createReceiverDirectory()) {
        _transport->setCacheDirectory(receiverDirectoryName());

        if (_transport->open(QIODevice::ReadOnly | QIODevice::WriteOnly)) {
            qDebug() << "The QNormFileTransport is open";
        } else {
            qCritical() << "Could not open the QNormFileTransport";
        }
    } else {
        qCritical() << "Could not create the receiver directory";
    }
}


NormFileTest::~NormFileTest() {
    delete _transport;
}

//
// Ask the UI for permission to receive the new file.
// If the UI grants permission, announce the new file to the UI.
// If the UI does not grant permission, cancel the new file.
//
void NormFileTest::on_newFile(QString fileName, NormObjectHandle object)
{
    QVariant message = tr("Accept file %1?").arg(fileName);
    QVariant no_text = tr("No");
    QVariant returnedValue;
    QVariant yes_text = tr("Yes");

    QMetaObject::invokeMethod(parent(), "dialog",
                              Q_RETURN_ARG(QVariant, returnedValue),
                              Q_ARG(QVariant, message),
                              Q_ARG(QVariant, yes_text),
                              Q_ARG(QVariant, no_text));

    if (returnedValue.toBool()) {
        message = tr("Receiving %1").arg(fileName);
        QMetaObject::invokeMethod(parent(), "addMessage",
                                  Q_ARG(QVariant, RECEIVER),
                                  Q_ARG(QVariant, message));
    } else {
        NormObjectCancel(object);
    }
}

//
// Tell the UI that NORM thinks it sent a file
//
void NormFileTest::on_objectSent(NormObjectHandle object)
{
    char fileName[PATH_MAX];
    //
    // BUG: assumes that object is a NormFile
    // This is partially a NORM bug because the NORM API
    // doesn't have type safe object handles.
    //
    if (NormFileGetName(object, fileName, PATH_MAX)) {
        QVariant message = tr("Sent %1").arg(fileName);
        //
        // It's safe to print fileName because NormFileGetName()
        // ensures that it is zero-terminated.
        //
        qDebug() << "Sent file " << fileName;
        QMetaObject::invokeMethod(parent(), "addMessage",
                                  Q_ARG(QVariant, SENDER),
                                  Q_ARG(QVariant, message));

    } else {
        qDebug() << "on_objectSent(): SHOULDN'T HAPPEN! Sent file but its name isn't valid.  This is a bug.";
    }
}

//
// Tell the UI that some of the files
// we have been receiving are now
// complete.
//
void NormFileTest::on_readyRead()
{
    QByteArray fileNameBytes = _transport->readLine();

    while (fileNameBytes.size() > 0) {
        QString fileName(fileNameBytes);
        QVariant message = tr("Received %1").arg(fileName);

        QMetaObject::invokeMethod(parent(), "addMessage",
                                  Q_ARG(QVariant, RECEIVER),
                                  Q_ARG(QVariant, message));

        qDebug() << "Received file " << fileName;
        fileNameBytes = _transport->readLine();
    }
}

//
// Tell the UI that it is free to send
// more files. The UI is listening for
// readyWrite().
//
void NormFileTest::on_readyWrite()
{
    emit readyWrite();
}

//
// Attempt to add url to the transmission queue.
// Return true if this is possible.  Return false otherwise.
//
bool NormFileTest::sendFile(QUrl url)
{
    bool answer = false;

    if (url.isValid() && url.isLocalFile()) {
        QString fileName = url.toLocalFile().append("\n");

        //
        // Remember! Each line we write into a QNormFileTransport
        // is the name of a file that we want NORM to send.
        //
        QByteArray fileNameByteArray = fileName.toLocal8Bit();

        //
        // Failure to send all of the bytes of fileNameByteArray
        // indicates that NORM couldn't enqueue the file in its
        // transmit queue.
        //
        answer = (fileNameByteArray.size() ==
                  _transport->write(fileNameByteArray.constData(),
                                    fileNameByteArray.size()));

        QVariant message;

        if (answer) {
            message = tr("Sending %1").arg(url.toString());
            qDebug() << "Sending " << url;
        } else {
            message = tr("Could not send %1").arg(url.toString());
            qDebug() << "Could not send " << url;
        }

        QMetaObject::invokeMethod(parent(), "addMessage",
                                  Q_ARG(QVariant, SENDER),
                                  Q_ARG(QVariant, message));

    }

    return answer;
}

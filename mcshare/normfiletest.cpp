#include "normfiletest.h"
#include "qnormfiletransporttest.h"
#include <QDebug>
#include <QDesktopServices>
#include <QVariant>

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

#define TX_1MBPS 1048576

NormFileTest::NormFileTest(QObject *parent) :
    QObject(parent)
{
    _transport = new QNormFileTransportTest(ADDR, PORT, NORM_NODE_ANY);
    _transport->setAuthority(this);
    NormSessionHandle session = _transport->normSession();

    // The default rate is 64kbps.
    // TODO: Choose a faster rate, 1mbps.
    // TODO: Define some constants for popular bit rates.
//    NormSetTxRate(session, TX_1MBPS);

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
    QObject::connect(_transport, SIGNAL(objectQueued(NormObjectHandle)),
                     this, SLOT(on_objectQueued(NormObjectHandle)));

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

bool NormFileTest::askToRenameFile(QString oldFileName, QString newFileName)
{
    QVariant message = tr("Rename file %1 as %2?").arg(oldFileName).arg(newFileName);
    QVariant no_text = tr("No");
    QVariant returnedValue;
    QVariant yes_text = tr("Yes");

    QMetaObject::invokeMethod(parent(), "dialog",
                              Q_RETURN_ARG(QVariant, returnedValue),
                              Q_ARG(QVariant, message),
                              Q_ARG(QVariant, yes_text),
                              Q_ARG(QVariant, no_text));

    return returnedValue.toBool();
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
// Tell the UI that sending is under way for some object.
//
void NormFileTest::on_objectQueued(NormObjectHandle object)
{
    char fileName[PATH_MAX];
    //
    // BUG: assumes that object is a NormFile
    // This is partially a NORM bug because the NORM API
    // doesn't have type safe object handles.
    //
    if (NormFileGetName(object, fileName, PATH_MAX)) {
        QVariant message = tr("Sending %1").arg(fileName);

        QMetaObject::invokeMethod(parent(), "addMessage",
                                  Q_ARG(QVariant, SENDER),
                                  Q_ARG(QVariant, message));
    } else {
        qDebug() << "on_sendingObject(): SHOULDN'T HAPPEN! Enqueued file but its name isn't valid.  This is a bug.";
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
// BUG: The caller can't distinguish the reason for the
// return value false.  It could mean that the transmit queue
// is temporarily full.  It could also mean that the URL is
// invalid.
//
// It might be useful to return false if and only if the
// transmit queue is temporarily full.  Otherwise, return
// true.
//
NormFileTest::Status NormFileTest::sendFile(QUrl url)
{
    Status answer = BadURL;

    qDebug() << "NormFileTest::sendFile(" << url << ")";

    if (url.isValid()) {
        bool couldWrite = false;
        QString fileName = url.toLocalFile();
        qDebug() << "NormFileTest::sendFile(): fileName " << fileName;
        fileName.append("\n");

        //
        // Remember! Each line we write into a QNormFileTransport
        // is the name of a file or directory that we want NORM to send.
        //
        QByteArray fileNameByteArray = fileName.toLocal8Bit();

        qDebug() << "NormFileTest::sendFile(): byteArray size "
                 << fileNameByteArray.size();
        //
        // Failure to send all of the bytes of fileNameByteArray
        // indicates that NORM couldn't enqueue the file in its
        // transmit queue.
        //
        const char *fileNameData = fileNameByteArray.constData();
        couldWrite = (fileNameByteArray.size() ==
                      _transport->write(fileNameData,
                                        fileNameByteArray.size()));

        QVariant message;

        if (couldWrite) {
            answer = Success;
            message = tr("Enqueued %1").arg(url.toString());
            qDebug() << "Enqueued " << url;
        } else {
            answer = Busy;
            message = tr("Could not enqueue %1").arg(url.toString());
            qDebug() << "Could not enqueue " << url;
        }

        QMetaObject::invokeMethod(parent(), "addMessage",
                                  Q_ARG(QVariant, SENDER),
                                  Q_ARG(QVariant, message));

    } else {
        qCritical() << "Invalid URL: " << url;
    }

    return answer;
}

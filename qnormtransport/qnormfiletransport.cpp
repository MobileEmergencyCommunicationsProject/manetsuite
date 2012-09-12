#include <protoDefs.h>
#include <QByteArray>
#include <QDebug>
#include <QFileInfo>
#include "qnormfiletransport.h"

#ifndef MIN
#define MIN(X,Y) ((X<Y)?(X):(Y))
#endif //!MIN

//
// Stream of files transported by NORM
//
// Write the name of a file into the stream to send the file.
//
// Read the name of a received file from the stream.
//
QNormFileTransport::QNormFileTransport(const char *address, unsigned short port, NormNodeId normNodeId, QObject *parent) :
    QNormTransport(address, port, normNodeId, parent)
{
}

QNormFileTransport::~QNormFileTransport()
{

}

// Put the name of the newly received file into _readBuffer.
void QNormFileTransport::appendToReadBuffer(NormEvent *event)
{
    char nameBuffer[PATH_MAX]; // PATH_MAX is defined in "protokit.h"
    bool validFileName = NormFileGetName(event->object,
                                         nameBuffer, PATH_MAX);
    if (validFileName) {
        QString fileName(nameBuffer);
        fileName.append('\n');
        _readBuffer.append(nameBuffer);
    } else {
        qCritical() << "QNormFileTransport::appendToReadbuffer() invalid file name";
    }
}

void QNormFileTransport::normRxObjectCompleted(NormEvent *event)
{
    NormObjectType objectType = NormObjectGetType(event->object);

    if (NORM_OBJECT_FILE == objectType) {

        // TODO: Consider getting the actual file name from
        // the NormObject's info, if there is any, and renaming
        // the file accordingly.
        //

        appendToReadBuffer(event);
        emit readyRead();
    } else {
        QString type;

        if (NORM_OBJECT_DATA == objectType) type = "OBJECT";
        else if (NORM_OBJECT_STREAM == objectType) type = "STREAM";
        else if (NORM_OBJECT_NONE == objectType) type = "INVALID";
        else type = QString("Unknown object type %1").arg(objectType);

        qCritical() << "QNormFileTransport::normRxObjectCompleted(): Received unexpected object: "
                    << type;
        NormObjectCancel(event->object);
    }
}

void QNormFileTransport::normRxObjectInfo(NormEvent *event) {
    // This code came from normApp.cpp.
    //
    // It would be nicer if we could rename the file
    // using Qt's file system abstractions.  Alas,
    // that work behind NORM's back and it might
    // confuse him.
    //
    // Rename rx file using newly received info
    QString fileName(cacheDirectory());
    fileName.append("/");

    char newName[PATH_MAX];
    UINT16 len = NormObjectGetInfoLength(event->object);
    len = MIN(len, PATH_MAX);
    NormObjectGetInfo(event->object, newName, len);

    fileName.append(newName);

    // Deal with concurrent rx name collisions
    // (TBD) and implement overwrite policy
    //       and cache files in cache mode

    if (! NormFileRename(event->object, fileName.toAscii().data()))
    {
        qCritical() << "QNormFileTransport::normRxObjectInfo(): Error renaming rx file: "
                    << fileName;
    }
}

void QNormFileTransport::normRxObjectNew(NormEvent *event)
{
    NormObjectType objectType = NormObjectGetType(event->object);

    if (NORM_OBJECT_FILE == objectType) {
        char nameBuffer[PATH_MAX]; // PATH_MAX is defined in "protokit.h"
        bool validFileName = NormFileGetName(event->object,
                                             nameBuffer, PATH_MAX);
        if (validFileName) {
            QString fileName(nameBuffer);

            // TODO: include event->object so that
            // the receiver of the newFile signal
            // can cancel the NormObject if it
            // does not wish to receive the file.
            emit newFile(fileName);
        } else {
            qCritical() << "QNormFileTransport::normRxObjectNew() NormEvent contained invalid file name";
            NormObjectCancel(event->object);
        }
    } else {
        QString type;

        if (NORM_OBJECT_DATA == objectType) type = "OBJECT";
        else if (NORM_OBJECT_STREAM == objectType) type = "STREAM";
        else if (NORM_OBJECT_NONE == objectType) type = "INVALID";
        else type = QString("Unknown object type %1").arg(objectType);

        qCritical() << "QNormFileTransport::normRxObjectNew(): Received unexpected object: "
                    << type;

        NormObjectCancel(event->object);
    }
}

void QNormFileTransport::normRxObjectUpdated(NormEvent *event)
{
    NormObjectType objectType = NormObjectGetType(event->object);

    if (NORM_OBJECT_FILE == objectType) {
        char nameBuffer[PATH_MAX]; // PATH_MAX is defined in "protokit.h"
        bool validFileName = NormFileGetName(event->object,
                                             nameBuffer, PATH_MAX);
        if (validFileName) {
            QString fileName(nameBuffer);

            // TODO: emit a signal that identifies the progress
            // in receiving this file.

            qDebug() << "QNormFileTransport::normRxObjectUpdated() update for file "
                     << fileName
                     << " size "
                     << NormObjectGetSize(event->object);
        } else {
            qCritical() << "QNormFileTransport::normRxObjectUpdated() NormEvent contained invalid file name";
            NormObjectCancel(event->object);
        }
    } else {
        QString type;

        if (NORM_OBJECT_DATA == objectType) type = "OBJECT";
        else if (NORM_OBJECT_STREAM == objectType) type = "STREAM";
        else if (NORM_OBJECT_NONE == objectType) type = "INVALID";
        else type = QString("Unknown object type %1").arg(objectType);

        qCritical() << "QNormFileTransport::normRxObjectNew(): Received unexpected object: "
                    << type;

        NormObjectCancel(event->object);
    }
}

void QNormFileTransport::normTxObjectSent(NormEvent *event)
{
    // TODO: enqueue the next file
    // TODO: emit a signal that identifies the end of transmission of the file.
}

void QNormFileTransport::normTxQueueEmpty(NormEvent *event)
{
    // XXX Is this the right place to emit readyWrite()?
    if (0 == _fileNameQueue.size()) {
        emit readyWrite();
    }

    sendFiles();
}

void QNormFileTransport::normTxQueueVacancy(NormEvent *event)
{
    // XXX Is this the right place to emit readyWrite()?
    if (0 == _fileNameQueue.size()) {
        emit readyWrite();
    }

    sendFiles();
}

// Enqueue names of files for transmission.
// Enqueue as many from _fileNameQueue as will fit.
void QNormFileTransport::sendFiles()
{
    while (_fileNameQueue.size() > 0) {
       const char *fileName = _fileNameQueue.first().constData();
       NormObjectHandle objectHandle =
               NormFileEnqueue(_normSessionHandle,
                               fileName,
                               // Send the file name and its length as
                               // the NORM_INFO.
                               fileName, _fileNameQueue.first().size() + 1);

        if (NORM_OBJECT_INVALID == objectHandle) {
            // Error handling.
            //
            // NormFileEnqueue() could fail because:
            //
            // 1. _normSessionHandle is not a sender.
            //    writeData() ensures that this is not the case.
            //
            // 2. NORM couldn't create a NormFileObject.
            //    This is an out-of-memory condition. Try later.
            //
            // 3. NORM couldn't open the file.
            //    The user doesn't have permission to read the file
            //    or the file doesn't exist. Reject this file.
            //
            //    The sender is supposed to ensure that the file
            //    is readable.  The file's permissions could have
            //    changed without the sender's knowledge.  We check
            //    here and complain if the file isn't readable.
            //

            QString fileName(_fileNameQueue.first().constData());
            QFileInfo info(fileName);
            if (! info.isReadable()) {
                qCritical() << "QNormFileTransport::sendFiles() skipping unreadable file "
                            << fileName;
                _fileNameQueue.removeFirst();
            } else {

                //
                // 4. NORM couldn't enqueue the file for transmission.
                //    The queue is full. Try later.
                //

                break;
            }
        } else {
            // TODO: emit a signal about the successful enqueuing of this file.

            qDebug() << "QNormFileTransport::sendFiles() enqueued file "
                     << _fileNameQueue.first().constData();
            _fileNameQueue.removeFirst();
        }
    }
}

// Expects null-terminated strings in *data.
// Consumes all such strings up to the last null.
// Returns that number.
//
// Ignores all bytes between the last null and
// data[len - 1].
//
qint64 QNormFileTransport::writeData(const char *data, qint64 len)
{
    qint64 answer = 0;

    // Don't do anything unless this QNormFileTransport is a sender
    // and there are some bytes to consume.
    if (isSender() && (len > 0) ) {
        QByteArray b(data, len);
        QList<QByteArray> fileNames = b.split(0);
        answer = len;

        if (0 != data[len -1]) { // XXX Is this a bug?
            qCritical("QNormFileTransport::writeData() data does not end in 0: %*s", len, data);
            answer -= fileNames.last().size();
            fileNames.removeLast();
        }

        if (fileNames.size() > 0) {
            _fileNameQueue.append(fileNames);
        }
    }

    return answer;
}


#include <protoDefs.h> // Defines PATH_MAX and MIN()
#include <QByteArray>
#include <QDebug>
#include <QDirIterator>
#include <QFileInfo>
#include "qnormfiletransport.h"

//
// Stream of files transported by NORM
//
// Write the name of a file into the stream to send the file.
//
// Read the name of a received file from the stream.
//
QNormFileTransport::QNormFileTransport(const char *address,
                                       unsigned short port,
                                       NormNodeId normNodeId,
                                       QObject *parent) :
    QNormTransport(address, port, normNodeId, parent),
    _resendCurrentFile(false)
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

        qCritical() << "QNormFileTransport::normRxObjectCompleted(): Received unexpected object: "
                    << QNormTransport::objectTypeToQString(objectType);
        NormObjectCancel(event->object);
    }
}

void QNormFileTransport::normRxObjectInfo(NormEvent *event) {
    // This code came from normApp.cpp.
    //
    // It would be nicer if we could rename the file
    // using Qt's file system abstractions.  Alas,
    // that works behind NORM's back and it might
    // confuse him.
    //
    // Rename rx file using newly received info
    const QDir *cacheDir = cacheDirectory();

    char newName[PATH_MAX];
    memset(newName, 0, PATH_MAX);
    UINT16 len = NormObjectGetInfoLength(event->object);
    len = MIN(len, PATH_MAX);
    NormObjectGetInfo(event->object, newName, len);

    newName[len] = '\0';
    QByteArray bytes(newName, len);

    if (len > 0) {
        QString newFileName = cacheDir->absoluteFilePath(newName);

        // TODO: Deal with concurrent rx name collisions
        //       and implement overwrite policy
        //       and cache files in cache mode
        //
        // TODO: This is a natural place to ask for user input
        //       about receiving the file, and about overwriting
        //       an existing file of the same name.
        //
        char nameBuffer[PATH_MAX]; // PATH_MAX is defined in <protoDefs.h>
        memset(nameBuffer, 0, PATH_MAX);
        bool validFileName = NormFileGetName(event->object,
                                             nameBuffer, PATH_MAX);
        if (validFileName) {
            QString oldFileName(nameBuffer);
            qDebug() << "QNormFileTransport::normRxObjectInfo(): Renaming "
                     << oldFileName << " to " << newFileName;

        } else {
            qCritical() << "QNormFileTransport::normRxObjectInfo(): Renaming <invalid file name> to "
                        << newFileName;
        }

        QByteArray newFileNameByteArray = newFileName.toLocal8Bit();
        if (! NormFileRename(event->object, newFileNameByteArray.constData()))
        {
            qCritical() << "QNormFileTransport::normRxObjectInfo(): Error renaming rx file: "
                        << newFileName;
        }
    } else {
        qWarning() << "QNormFileTransport::normRxObjectInfo(): It has info of length " << len;
    }
}

void QNormFileTransport::normRxObjectNew(NormEvent *event)
{
    NormObjectType objectType = NormObjectGetType(event->object);

//    qDebug() << "QNormFileTransport::normRxObjectNew()";

    if (NORM_OBJECT_FILE == objectType) {
        char nameBuffer[PATH_MAX]; // PATH_MAX is defined in <protoDefs.h>
        memset(nameBuffer, 0, PATH_MAX);
        bool validFileName = NormFileGetName(event->object,
                                             nameBuffer, PATH_MAX);

        if (NormObjectHasInfo(event->object)) {
            char *infoBuffer;
            UINT16 infoLength = NormObjectGetInfoLength(event->object);
            infoBuffer = new char[infoLength+1]; // +1 byte for the terminating zero.
            memset(infoBuffer, 0, infoLength + 1);
            NormObjectGetInfo(event->object, infoBuffer, infoLength);
            QByteArray bytes(infoBuffer, infoLength);

            qDebug() << "QNormFileTransport::normRxObjectNew(): It has info of length " << infoLength;
            qDebug() << "QNormFileTransport::normRxObjectNew():" << infoBuffer;
            qDebug() << "QNormFileTransport::normRxObjectNew(): bytes: " << bytes;
            qDebug() << "QNormFileTransport::normRxObjectNew(): hex: " << bytes.toHex();
            delete infoBuffer;
        }

        // XXX At this point, the file probably has a temporary name
        //     supplied by the NORM protocol engine.  If that's so,
        //     we'll have to notify the user again when the file's
        //     real name arrives in normRxObjectInfo().
        //
        if (validFileName) {
            QString fileName(nameBuffer);
            qDebug() << "QNormFileTransport::normRxObjectNew() " << fileName;
            emit newFile(fileName, event->object);
        } else {
            qCritical() << "QNormFileTransport::normRxObjectNew() NormEvent contained invalid file name";
            NormObjectCancel(event->object);
        }
    } else {

        qCritical() << "QNormFileTransport::normRxObjectNew(): Received unexpected object: "
                    << QNormTransport::objectTypeToQString(objectType);

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

//            qDebug() << "QNormFileTransport::normRxObjectUpdated() update for file "
//                     << fileName
//                     << " size "
//                     << NormObjectGetSize(event->object);
        } else {
            qCritical() << "QNormFileTransport::normRxObjectUpdated() NormEvent contained invalid file name";
            NormObjectCancel(event->object);
        }
    } else {

        qCritical() << "QNormFileTransport::normRxObjectUpdated(): Received unexpected object: "
                    << QNormTransport::objectTypeToQString(objectType);

        NormObjectCancel(event->object);
    }
}

void QNormFileTransport::normTxObjectSent(NormEvent *event)
{
    char nameBuffer[PATH_MAX];
    // BUG: assumes that object is a NormFile
    if (NormFileGetName(event->object, nameBuffer, PATH_MAX)) {
        QString fileName(nameBuffer);
        //
        // It's safe to print fileName because NormFileGetName()
        // ensures that it is zero-terminated.
        //
        qDebug() << "normTxObjectSent(): Sent file " << fileName;
    } else {
        qDebug() << "normTxObjectSent(): SHOULDN'T HAPPEN! Sent file but its name isn't valid.  This is a bug.";
    }

    emit objectSent(event->object);
}

void QNormFileTransport::normTxQueueEmpty(NormEvent *event)
{
    // XXX Will normTxQueueEmpty() and normTxQueueVacancy() as implemented
    //     keep norm's tx queue sufficiently full? Will this implementation
    //     miss any opportunity to write?
    //
    emit readyWrite();

    sendFiles();
}

void QNormFileTransport::normTxQueueVacancy(NormEvent *event)
{
    // XXX Will normTxQueueEmpty() and normTxQueueVacancy() as implemented
    //     keep norm's tx queue sufficiently full? Will this implementation
    //     miss any opportunity to write?
    //
    emit readyWrite();

    sendFiles();
}

// Enqueue names of files for transmission.
// Enqueue as many from _fileIterator as will fit.
void QNormFileTransport::sendFiles()
{
    while (_fileIterator.hasNext()) {
        QString fileName;

        if (_resendCurrentFile) {
            fileName = _fileIterator.current();
        } else {
            fileName = _fileIterator.next();
        }

        _resendCurrentFile = false;

        // Note the steps that Qt requires to convert a QString
        // into char *.  They must be performed separately.
        // If you combine them
        //
        // const char *fileNameData = fileName.toLocal8Bit().data();
        //
        // you will get unexpected results.
        //
        // See http://qt-project.org/faq/answer/how_can_i_convert_a_qstring_to_char_and_vice_versa
        //
        QByteArray fileNameByteArray = fileName.toLocal8Bit();
        const char *fileNameData = fileNameByteArray.data();

        QString fileNameInfo = _fileIterator.info();
        QByteArray fileNameInfoByteArray = fileNameInfo.toLocal8Bit();
        const char *fileNameInfoData = fileNameInfoByteArray.data();

        NormObjectHandle objectHandle =
                NormFileEnqueue(_normSessionHandle,
                                fileNameData,
                                fileNameInfoData, strlen(fileNameInfoData));

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
            //    There is no way to distinguish between this and #4.
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
            // 4. NORM couldn't enqueue the file for transmission.
            //    The queue is full. Try later.
            //    There is no way to distinguish between this and #2.
            //

            // Case #3: Can't open the file.
            QFileInfo info(fileName);
            if (!info.exists() || !info.isReadable() || !info.isFile()) {
                qCritical() << "QNormFileTransport::sendFiles() skipping non-existent or unreadable file or directory "
                            << fileName;
            } else {
                // Case #2 and case #4: assume norm's tx queue is full.
                // Wait for normTxQueueEmpty() or normTxQueueVacancy().
                _resendCurrentFile = true;
                break;
            }
        } else {
            emit objectQueued(objectHandle);

            qDebug() << "QNormFileTransport::sendFiles() enqueued file "
                     << fileName;
        }
    }
}

// Expects newline-terminated strings in *data.
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
        QList<QByteArray> fileNames = b.split('\n');
        answer = len;

        if (fileNames.length() > 0) {
            if ('\n' != data[len -1]) { // XXX Does this condition indicate a bug?
                qCritical("QNormFileTransport::writeData() data does not end in \\n: %*s", len, data);
                answer -= fileNames.last().size();
                fileNames.removeLast();
            }
        }

        //
        // QByteArray.split("abcde\n") produces a QList<QByteArray> of two elements:
        // "abcde" and "".  The last one isn't a valid file name, so throw it away.
        //
        if (fileNames.length() > 0) {
            if (0 == fileNames.last().length()) {
                fileNames.removeLast();
            }
        }

        // TODO: Consider removing all zero-length elements of fileNames.

        for (int i=0; i < fileNames.size(); i++) {
            QString s(fileNames.at(i));
            _fileIterator.append(s);
        }
    }

    sendFiles();

    return answer;
}


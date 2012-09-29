#include <protoDefs.h> // Defines PATH_MAX and MIN()
#include <QDebug>
#include "qnormfiletransporttest.h"

QNormFileTransportTest::QNormFileTransportTest(const char* address,
                                               unsigned short port,
                                               NormNodeId normNodeId,
                                               QObject *parent) :
    QNormFileTransport(address, port, normNodeId, parent),
    _authority(0)
{
}

QNormFileTransportTest::~QNormFileTransportTest()
{
}


/*
  Ask the authority for permission to rename a file.
  If it is granted or if there is no authority, delegate
  the renaming task to the base class.

  Scenario: We're receiving a file.  It has a temporary name.
  A NormRxObjectInfo event arrives. This event contains
  the real name of the file. The base class always renames the
  file.
  */
void QNormFileTransportTest::normRxObjectInfo(NormEvent *event) {
    bool permission = true;
    QString oldFileName;
    QString newFileName;

    if (0 != _authority) {
        //
        // Rename rx file using newly received info
        const QDir *cacheDir = cacheDirectory();

        char newName[PATH_MAX];
        UINT16 len = NormObjectGetInfoLength(event->object);
        len = MIN(len, PATH_MAX);
        NormObjectGetInfo(event->object, newName, len);

        newFileName = cacheDir->absoluteFilePath(newName);

        // TODO: Deal with concurrent rx name collisions
        //       and implement overwrite policy
        //       and cache files in cache mode
        //
        // TODO: This is a natural place to ask for user input
        //       about receiving the file, and about overwriting
        //       an existing file of the same name.
        //
        char nameBuffer[PATH_MAX]; // PATH_MAX is defined in <protoDefs.h>
        bool validFileName = NormFileGetName(event->object,
                                             nameBuffer, PATH_MAX);
        if (validFileName) {
            oldFileName = QString(nameBuffer);

            permission = _authority->askToRenameFile(oldFileName, newFileName);


        } else {

            qCritical() << "QNormFileTransportTest::normRxObjectInfo(): Renaming <invalid file name> to "
                        << newFileName;
        }
    }

    if (permission) {

        qDebug() << "QNormFileTransportTest::normRxObjectInfo(): Renaming "
                 << oldFileName << " to " << newFileName;

        QNormFileTransport::normRxObjectInfo(event);
    } else {
        qDebug() << "QNormFileTransportTest::normRxObjectInfo(): Permission denied! Cancelling "
                 << oldFileName;
        NormObjectCancel(event->object);
    }
}

void QNormFileTransportTest::setAuthority(Authority *authority)
{
    _authority = authority;
}

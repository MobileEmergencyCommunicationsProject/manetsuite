#include "applogic.h"
#include <QDebug>
#include <QSettings>
#include <QUuid>

#define ADDR "224.1.2.3"
#define INTERFACE "wlan0"
#define PORT 6004

AppLogic::AppLogic(QObject *parent) :
    QObject(parent)
{
    // TODO: Start _transport as sender and receiver

    QSettings settings("mecp", "balls");
    _playerID = settings.value("PlayerID").toString();
    qDebug() << "AppLogic::AppLogic(): _playerID" << _playerID;

    if ("" == _playerID) {
        _playerID = uuid();
        settings.setValue("PlayerID", _playerID);
        qDebug() << "AppLogic::AppLogic(): Created new _playerID:" << _playerID;
    }

    _transport = new QNormDataTransport(ADDR, PORT, NORM_NODE_ANY);
    QObject::connect(_transport, SIGNAL(readyRead()),
                     this, SLOT(on_readyRead()));

    _stream.setDevice(_transport);
}

void AppLogic::dispatchEvent(qint64 type, qint64 length, void *value)
{
    switch (type) {
    case 1: // ADD
        qDebug() << "AppLogic::displatchEvent(): ADD";
        break;
    case 2: // REMOVE
        qDebug() << "AppLogic::dispatchEvent(): REMOVE";
        break;
    default:
        qCritical() << "AppLogic::dispatchEvent(): unknown type: " << type;
    }
}

void AppLogic::on_add(QString uuid, QString color, int duration, int velocity, double x, double y, double z)
{
    // Send ADD to mcast group
    qDebug() << "AppLogic::on_add(" << uuid << ", " << color << ", " << duration << ", " << velocity << ", "
             << x << ", " << y << ", " << z << ")";
}

void AppLogic::on_readyRead()
{
    // Read from the QNormTransport
    qint64 objectLength;

    _stream >> objectLength;

    if (0 >= objectLength) {
        // Can read objectLength bytes.
        // From those bytes, extract the event.
        // Dispatch the event
        //
        //
        // Events are structured this way:
        //
        // qint64 type
        // qint64 length
        // char *value

        if (sizeof(qint64) <= objectLength) { // Can read a qint64
            qint64 type;

            _stream >> type;

            objectLength -= sizeof(qint64);

            if (sizeof(qint64) <= objectLength) { // Can read a qint64
                qint64 length;

                _stream >> length;

                objectLength -= sizeof(qint64);

                // Have type, length.  Now need value.

                // At this point, length and objectLength must be equal.

                if (objectLength == length) {
                    int numRead = 0;
                    char *value = 0;

                    if (0 < length) {
                        value = new char[length];
                        numRead = _stream.readRawData(value, length);
                    }

                    if (numRead == length) {
                        dispatchEvent(type, length, value);
                        delete[] value;
                    } else if (-1 == numRead) {
                        qCritical() << "AppLogic::on_readyRead(): error reading "
                                    << length << " bytes of value.";
                    } else {
                        // short read
                        qCritical() << "AppLogic::on_readyRead(): short read! Expected "
                                    << length << " bytes.  Read "
                                    << numRead;
                    }

                } else {
                    // some error
                    qCritical() << "AppLogic::on_readyRead(): Framing error! Read type (len="
                                << sizeof(qint64) << ", val=" << type
                                << ") and length(len=" << sizeof(qint64) << ", val=" << length
                                << "). Frame length suggests there should be " << objectLength << " bytes.";
                }
            } else {
                qCritical() << "AppLogic::on_readyRead(): expected to read length in "
                            << sizeof(qint64) << " bytes but there are only"
                            << objectLength << " bytes left in the NORM object.";
            }
        } else {
            qCritical() << "AppLogic::on_readyRead(): expected to read type in "
                        << sizeof(qint64) << " bytes but there are only "
                        << objectLength << " bytes in the NORM object.";
        }
    }
}

void AppLogic::on_readyWrite()
{
    // Write a message from the queue to the QNormTransport
    if (! _messagesForGroup.empty()) {
        QString message = _messagesForGroup.first();
        QByteArray messageData = message.toLocal8Bit();
        int bytesWritten = _transport->write(messageData);

        if (messageData.length() == bytesWritten) {
            _messagesForGroup.removeFirst();
        } else {
            qCritical() << "AppLogic::on_readyWrite(): Short write: "
                        << bytesWritten << " instead of " << messageData.length()
                        << ": " << message;
        }
    }
}

void AppLogic::on_remove(QString uuid)
{
    // Send REMOVE to mcast group
    qDebug() << "AppLogic::on_remove(" << uuid << ")";
}

QString AppLogic::playerID()
{
    return _playerID;
}

QString AppLogic::uuid()
{
    QUuid id = QUuid::createUuid();
    return id.toString();
}

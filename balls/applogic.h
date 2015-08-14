#ifndef APPLOGIC_H
#define APPLOGIC_H

#include <QDataStream>
#include <QNormDataTransport>
#include <QObject>

// AppLogic provides communication and support services to the UI.
//
// AppLogic sends ADD and REMOVE messages from the UI to a
// multicast group.  The UI raises signals that are connected to
// AppLogic's on_add() and on_remove() slots.
//
// AppLogic receives ADD and REMOVE messages from
// the multicast group and delivers them to the UI. It delivers them
// by raising the add() and remove() signals, respectively.
//
// AppLogic creates universally unique ids for the UI.
//
class AppLogic : public QObject
{
    Q_OBJECT
public:
    explicit AppLogic(QObject *parent = 0);

    // Return a QUuid in a form that QML can use
    Q_INVOKABLE QString uuid();

    Q_PROPERTY(QString playerID READ playerID)

    QString playerID();

signals:
    void add(QString uuid, QString color, int duration, int velocity, double x, double y, double z);
    void remove(QString uuid);

public slots:
    void on_add(QString uuid, QString color, int duration, int velocity, double x, double y, double z);
    void on_readyRead();
    void on_readyWrite();
    void on_remove(QString uuid);

private:
    // Messages waiting to be sent to the mcast group
    QList<QString> _messagesForGroup;
    QString _playerID;
    QDataStream _stream;
    QNormDataTransport *_transport;

    void dispatchEvent(qint64 type, qint64 length, void *value);

};

#endif // APPLOGIC_H

#ifndef NORMFILETEST_H
#define NORMFILETEST_H

#include <QDeclarativeItem>
#include <QNormFileTransport>
#include <QObject>
#include <QUrl>

/*
  NormFileTest is the application logic for an interactive test of
  NormFileTransport. The UI is written in QML.

  This NormFileTest expects to be a child of a QML object. It expects the
  QML object to have functions:

  dialog(message, yes_button, no_button)

     Displays a dialog containing message and two buttons with text
     yes_button and no_button respectively. The dialog returns a boolean
     value, true if the user selected the yes button and false if the user
     selected the no button.

  addMessage(destination, message)

     Appends message to the QML object designated by destination. Supported
     values of destination are "receiver" and "sender".


  It expects the parent to connect to the onReadyWrite signal. The signal
  indicates that it is safe to invoke this NormFileTest's sendFile(QUlr).
*/
class NormFileTest : public QObject
{
    Q_OBJECT

public:
    explicit NormFileTest(QObject *parent = 0);
    virtual ~NormFileTest();

    // Add url to the transmission queue.
    Q_INVOKABLE bool sendFile(QUrl url);

signals:
    // The app has space in the transmission queue.
    void readyWrite();

public slots:
    void on_newFile(QString fileName, NormObjectHandle object);
    void on_objectSent(NormObjectHandle object);
    void on_readyRead();
    void on_readyWrite();

private:
    QNormFileTransport *_transport;
};

#endif // NORMFILETEST_H

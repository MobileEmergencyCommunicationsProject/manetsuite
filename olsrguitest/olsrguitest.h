#ifndef OLSRGUITEST_H
#define OLSRGUITEST_H

#include <QObject>
#include <QPipe>
#include <QStringList>

class OlsrGuiTest : public QObject
{
    Q_OBJECT
public:
    explicit OlsrGuiTest(QObject *parent = 0);

signals:
    void newMessage(QString message, QString color);

public slots:
    void start();
    void stop();

private slots:
    // for signals from serverPipe
    void on_readyRead();

private:
    QString nextColor();
    void onClientConnected();
    bool ProcessCommands(QStringList commands);
    QString randomAddress();
    QString randomNeighbors(int numNeighbors);
    QString randomRoutes(int numRoutes);
    QString randomStatus();

    void SendGuiNeighbors();
    void SendGuiRoutes();
    void SendGuiSettings();
    void sendToServer(QString command);
    bool StringProcessCommands(char* theString);

    // Message colors alternate between two colors based on _colorValue
    bool _colorValue;
    // Send commands to the OLSR GUI on pipeToServer
    QPipe pipeToServer;
    QString serverPipeName;

    // Read commands (as a server) on pipeFromClient
    QPipe pipeFromClient;
    QString listeningPipeName;
};

#endif // OLSRGUITEST_H

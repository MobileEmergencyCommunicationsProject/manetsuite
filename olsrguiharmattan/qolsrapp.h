#ifndef QOLSRAPP_H
#define QOLSRAPP_H

#include <QPipe>
#include <QStringList>
#include <QTimer>

class QOLSRApp : public QObject
{
    Q_OBJECT

public:
    QOLSRApp();
    ~QOLSRApp();

signals:
    // These signals inform the UI of changes in
    // routing, neighbor, and protocol settings.
    //
    // Add one route to the display
    void addDestEntry(const QString &address, const QString &gateway, const QString &weight, const QString &interfaceName);
    // Add one neighbor to the display
    void addNeighborEntry(const QString &address, const QString &status, const QString &hysteresis, const QString &mPRSelect);
    // Remove all routes from the display
    void clearDestEntries();
    // Remove all neighbors from the display
    void clearNeighborEntries();
    // Apply new settings to the display.
    void setSettings(const bool al, const bool fuzzy, const bool slowdown,
                     const double hi, const double hj, const double ht,
                     const double tci, const double tcj, const double tct,
                     const double hnai, const double hnaj, const double hnat,
                     const double up, const double down, const double alpha,
                     const int willingness);

public slots:
    // With these slots, the UI asks for the current
    // routing, neighbor, and protocol settings.
    //
    // Client asks for current settings. Deliver with setSettings().
    void onGetSettings();

    // Client asks to reconnect to olsrd.
    void onReconnect();

    // Client delivers new values of settings.
    void onSettingsChanged(const bool al, const bool fuzzy, const bool slowdown,
                           const double hi, const double hj, const double ht,
                           const double tci, const double tcj, const double tct,
                           const double hnai, const double hnaj, const double hnat,
                           const double up, const double down, const double alpha,
                           const int willingness);

    // Client asks for current neighbors.
    void onUpdateNeighbors();

    // Client asks for current routes.
    void onUpdateRoutes();

private slots:
    // for signals from serverPipe
    void on_readyRead();

    // for update_timer
    void onUpdateTimerTimeout();

private:
    bool ProcessCommands(QStringList commands);
    void sendToServer(QString command);
    bool StringProcessCommands(char* theString);

    // Send commands to the OLSR server on pipeToServer
    QPipe pipeToServer;
    QString serverPipeName;

    // Read commands (as a server) on pipeFromClient
    QPipe pipeFromClient;
    QString listeningPipeName;
    int updateInterval;
    QTimer updateTimer;

};
#endif // QOLSRAPP_H

#ifndef QOLSRAPP_H
#define QOLSRAPP_H

#include <QLocalServer>
#include <QLocalSocket>
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
    void addDestEntry(const QString &dest, const QString &gw, const QString &weight, const QString &interfaceName);
    // Add one neighbor to the display
    void addNeighborEntry(const QString &neighbor, const QString &type, const QString &hysterisis, const QString &mPRSelect);
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
    // For signals from client_pipe
    void onClientConnected();
    void onClientDisconnected();
    void onClientError(QLocalSocket::LocalSocketError socketError);
    void onClientStateChanged(QLocalSocket::LocalSocketState socketState);

    // for signals from server_pipe
    void onServerNewConnection();

    // for update_timer
    void onUpdateTimerTimeout();

private:
    bool ProcessCommands(QStringList commands);
    bool StringProcessCommands(char* theString);

    QLocalSocket clientPipe;
    QString clientPipeName;
    QLocalServer serverPipe;
    QString serverPipeName;
    int updateInterval;
    QTimer updateTimer;

};
#endif // QOLSRAPP_H

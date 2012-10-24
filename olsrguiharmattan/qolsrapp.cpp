#include <errno.h>
#include <QDebug>
#include <QDir>
#include "qolsrapp.h"
#include <QStringList>

QOLSRApp::QOLSRApp(): /*serverPipeName(QDir::tempPath() + "/nrlolsr")*/
    serverPipeName("/tmp/nrlolsr"),
    listeningPipeName(QDir::tempPath() + "/nrlolsrgui"),
//    listeningPipeName("/tmp/nrlolsrgui"),
    updateInterval(5000 /*milliseconds*/)
{
    updateTimer.setInterval(updateInterval);
    connect(&updateTimer, SIGNAL(timeout()),
            this, SLOT(onUpdateTimerTimeout()));
    connect(&pipeFromClient, SIGNAL(readyRead()),
            this, SLOT(on_readyRead()));

    onReconnect();
//    updateTimer.start();
}

QOLSRApp::~QOLSRApp()
{
    updateTimer.stop();
    pipeFromClient.close();
    pipeToServer.close();
}

void QOLSRApp::onGetSettings()
{
    //send request to nrlolsr
    sendToServer("-sendGuiSettings");
}

void QOLSRApp::on_readyRead()
{
    qint64 len = 8*1024; // XXX Magic constant 8K
    char buffer[len];
    qint64 bytesRead;

    // TODO: Consider allocating a buffer big enough
    // for every byte currently waiting to be read from
    // serverPipe.  serverPipe.bytesAvailable() tells
    // how many.  Can that ever be too many bytes
    // to allocate on the device at once?
    memset(buffer, 0, len);
    bytesRead = pipeFromClient.read(buffer, len);
    if (-1 != bytesRead) {
        StringProcessCommands(buffer);
    } else {
        qCritical() << "QOLSRApp::on_readyRead(): read() returned -1";
    }
}

void QOLSRApp::onReconnect()
{
    qDebug() << "QOLSRApp::onReconnect()";
    if (pipeToServer.isOpen()) {
        qDebug() << "QOLSRApp::onReconnect(): closing pipeToServer";
        pipeToServer.close();
    }

    if (pipeFromClient.isOpen()) {
        qDebug() << "QOLSRApp::onReconnect(): closing pipeFromClient";
        pipeFromClient.close();
    }

    if (-1 != pipeFromClient.listen(listeningPipeName)) { //pipe to receive messages

        if (-1 != pipeToServer.connect(serverPipeName)) { //send pipe to nrlolsr
            //send the server pipe name to the client so it can connect back
            QString command("guiClientStart ");
            command.append(listeningPipeName);

            sendToServer(command);
            onGetSettings();
            onUpdateRoutes();
            onUpdateNeighbors();
        } else {
            // error
            qCritical() << "QOLSRApp::onReconnect(): connect(" << serverPipeName
                        << "): error ("
                        << errno << "): " << strerror(errno);
            pipeFromClient.close();
            pipeToServer.close();
        }
    } else {
        qCritical() << "QOLSRApp::onReconnect(): listen(" << listeningPipeName
                    << "): error ("
                    << errno << "): " << strerror(errno);
        pipeFromClient.close();
    }
}


// Willingness is UINT8, not bool.
void QOLSRApp::onSettingsChanged(const bool al, const bool fuzzy, const bool slowdown,
                                 const double hi, const double hj, const double ht,
                                 const double tci, const double tcj, const double tct,
                                 const double hnai, const double hnaj, const double hnat,
                                 const double up, const double down, const double alpha,
                                 const int willingness)
{// willingness is ignored here because the original NRL OLSR GUI ignores it.
    QString command = QString("-al %1 -fuzzy %2 -slowdown %3 -hi %4 -hj %5 -ht %6 -tci %7 -tcj %8 -tct %9 -hnai %10 -hnaj %11 -hnat %12 -hys up %13 -hys down %14 -hys alpha %15 -w %16")
            .arg((al ? "on":"off")).arg((fuzzy ? "on":"off")).arg((slowdown ? "on":"off")).arg(hi).arg(hj).arg(ht).arg(tci).arg(tcj).arg(tct).
            arg(hnai).arg(hnaj).arg(hnat).arg(up).arg(down).arg(alpha).arg(willingness);
    sendToServer(command);
}

void QOLSRApp::onUpdateNeighbors()
{
    //send request to nrlolsr
    sendToServer("-sendGuiNeighbors");
    emit clearNeighborEntries();
}

void QOLSRApp::onUpdateRoutes()
{
    //send request to nrlolsr
    sendToServer("-sendGuiRoutes");
    emit clearDestEntries();
}

void QOLSRApp::onUpdateTimerTimeout()
{
    onUpdateRoutes();
    onUpdateNeighbors();
}

bool QOLSRApp::ProcessCommands(QStringList argv)
{
    for(int i=0; i < argv.length(); i++){
        if(argv.at(i) == "guiServerStart") {
            i++;
            if (argv.at(i) != serverPipeName) {
                pipeToServer.close();
                pipeToServer.connect(argv.at(i));
                serverPipeName = argv.at(i);
                // TODO: send warning text to UI.
                qWarning() << "QOLSRApp::ProcessCommands(): Connecting to new server, "
                           << argv.at(i);
            } else {
                onGetSettings();
                onUpdateRoutes();
                onUpdateNeighbors();
            }
        } else if( argv.at(i)== "routes"){
            // nrlolsrd sends: routes [<destination> <gateway> <weight> <interface>]* end-routes
            // <weight> is a double.  The rest are strings.

            emit clearDestEntries();
            i++;
            for(; i+4< argv.length(); i+=4){
                emit addDestEntry(argv.at(i), argv.at(i+1), argv.at(i+2), argv.at(i+3));
            }
        } else if(argv.at(i) == "neighbors") {
            // nrlolsrd sends: neighbors [<address> <status> <connectivity> <mpr selector>]* end-neighbors
            // <address> is a string.
            // <status> is one of: LOST, ASYM, SYM, MPR, PENDING, INVALID
            // <connectivity> is a double
            // <mpr selector> is one of: TRUE, FALSE
            //
            // NRL's olsrgui calls <connectivity> "hysterisis".

            emit clearNeighborEntries();
            i++;
            //add loop to add entries
            for(; i+4< argv.length(); i+=4){
                emit addNeighborEntry(argv.at(i+ 0), argv.at(i+ 1), argv.at(i+ 2), argv.at(i+ 3));
            }
        } else if(argv.at(i) == "settings"){
            // nrlolsrd sends: settings <all links> <fuzzy flooding> <tc slow down> \
            // <hello interval> <hello jitter> <hello timeout factor> \
            // <tc interval> <tc jitter> <tc timeout factor> \
            // <hna interval> <hna jitter> <hna timeout factor> \
            // <hysteresis up> <hysteresis down> <hysteresis alpha> <local willingness>
            //
            // <all links>, <fuzzy flooding>, and <tc slow down> are ints. They represent bools.
            // <local willingness> is an int.
            // The rest are doubles.
            i++;
            bool al = (argv.at(i+0).toInt() != 0);
            bool fuzzy = (argv.at(i+1).toInt() != 0);
            bool slowdown = (argv.at(i+2).toInt() != 0);

            qDebug() << "QOLSRApp::processCommands(): settings "
                     << al << ", " << fuzzy << ", " << slowdown << ", "
                     << argv.at(i+ 3) << ", " << argv.at(i+ 4) << ", " << argv.at(i+ 5) << ", " //hi hj ht
                     << argv.at(i+ 6) << ", " << argv.at(i+ 7) << ", " << argv.at(i+ 8) << ", " //tci tcj tct
                     << argv.at(i+ 9) << ", " << argv.at(i+10) << ", " << argv.at(i+11) << ", " //hnai hnaj hnat
                     << argv.at(i+12) << ", " << argv.at(i+13) << ", " << argv.at(i+14) << ", " //up down alpha
                     << argv.at(i+15);

            emit setSettings(al, fuzzy, slowdown, //al fuzzy slowdown
                             argv.at(i+ 3).toDouble(), argv.at(i+ 4).toDouble(), argv.at(i+ 5).toDouble(), //hi hj ht
                             argv.at(i+ 6).toDouble(), argv.at(i+ 7).toDouble(), argv.at(i+ 8).toDouble(), //tci tcj tct
                             argv.at(i+ 9).toDouble(), argv.at(i+10).toDouble(), argv.at(i+11).toDouble(), //hnai hnaj hnat
                             argv.at(i+12).toDouble(), argv.at(i+13).toDouble(), argv.at(i+14).toDouble(), //up down alpha
                             argv.at(i+15).toInt()); //willingness
            i+=16;
        }
    }

    return true;
}

void QOLSRApp::sendToServer(QString command)
{
    if (command.length() > 0) {
        QByteArray bytes = command.toLocal8Bit();
        qint64 bytesWritten = pipeToServer.write(bytes);

        if (bytesWritten == bytes.length()) {
            qDebug() << "QOLSRApp::on_readyWrite(): sent command: "
                     << command;
        } else {
            qCritical() << "QOLSRApp::on_readyWrite(): short write: "
                        << bytesWritten << " instead of " << bytes.length()
                        << "(" << command << ")";
        }
    }
}

bool QOLSRApp::StringProcessCommands(char* theString)
{
    QString commandString(theString);
    QStringList commands = commandString.split(" ");
    bool returnvalue = ProcessCommands(commands);

    return returnvalue;
}


#include <QDebug>
#include <QDir>
#include "qolsrapp.h"
#include <QStringList>

QOLSRApp::QOLSRApp(): /*clientPipeName(QDir::tempPath() + "/nrlolsr")*/
    serverPipeName("/tmp/nrlolsr"),
//    serverPipeName(QDir::tempPath() + "/nrlolsrgui"),
    listeningPipeName("/tmp/nrlolsrgui"),
    updateInterval(5000 /*milliseconds*/)
{
    updateTimer.setInterval(updateInterval);
    connect(&updateTimer, SIGNAL(timeout()),
            this, SLOT(onUpdateTimerTimeout()));
    connect(&pipeFromClient, SIGNAL(readyRead()),
            this, SLOT(on_readyRead()));
    connect(&pipeToServer, SIGNAL(readyWrite()),
            this, SLOT(on_readyWrite()));

    onClientConnected();
    pipeFromClient.listen(listeningPipeName); //pipe to receive messages
    pipeToServer.connect(serverPipeName); //send pipe to nrlolsr
//    updateTimer.start();
}

QOLSRApp::~QOLSRApp()
{
    updateTimer.stop();
    pipeFromClient.close();
    pipeToServer.close();
}

void QOLSRApp::onClientConnected()
{
    //send the server pipe name to the client so it can connect back
    QString command("guiClientStart ");
    command.append(listeningPipeName);

    _commandsForServer.append(command);
    onGetSettings();
    onUpdateRoutes();
    onUpdateNeighbors();
}

void QOLSRApp::onGetSettings()
{
    //send request to nrlolsr
    _commandsForServer.append("-sendGuiSettings");
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
    StringProcessCommands(buffer);
}

void QOLSRApp::on_readyWrite()
{
    if (_commandsForServer.length() > 0) {
        QString first = _commandsForServer.first();
        qint64 bytesWritten = pipeToServer.write(first.toLocal8Bit());

        // TODO: If we couldn't write all of first,
        // consider removing the bytes we did write
        // and leave the rest of first at the head of
        // _commandsForServer
        if (bytesWritten == first.length()) {
            qDebug() << "QOLSRApp::on_readyWrite(): sent command: "
                     << first;
            _commandsForServer.removeFirst();
        } else {
            // BUG: This could put us in an infinite loop.
            // What if we can never write all of first?
            // Then we will never leave this short write condition.
            qCritical() << "QOLSRApp::on_readyWrite(): short write: "
                        << bytesWritten << " instead of " << first.length();
        }
    }
}

void QOLSRApp::onSettingsChanged(const bool al, const bool fuzzy, const bool slowdown,
                                 const double hi, const double hj, const double ht,
                                 const double tci, const double tcj, const double tct,
                                 const double hnai, const double hnaj, const double hnat,
                                 const double up, const double down, const double alpha,
                                 const bool willingness)
{// willingness is ignored here because the original NRL OLSR GUI ignores it.
    QString command = QString("-al %1 -fuzzy %2 -slowdown %3 -hi %4 -hj %5 -ht %6 -tci %7 -tcj %8 -tct %9 -hnai %10 -hnaj %11 -hnat %12 -hys up %13 -hys down %14 -hys alpha %15").arg(al).arg(fuzzy).arg(slowdown).arg(hi).arg(hj).arg(ht).arg(tci).arg(tcj).arg(tct).arg(hnai).arg(hnaj).arg(hnat).arg(up).arg(down).arg(alpha);
    _commandsForServer.append(command);
}

void QOLSRApp::onUpdateNeighbors()
{
    //send request to nrlolsr
    _commandsForServer.append("-sendGuiNeighbors");
    emit clearNeighborEntries();
}

void QOLSRApp::onUpdateRoutes()
{
    //send request to nrlolsr
    _commandsForServer.append("-sendGuiRoutes");
    emit clearDestEntries();
}

void QOLSRApp::onUpdateTimerTimeout()
{
    onUpdateRoutes();
    onUpdateNeighbors();
}

bool QOLSRApp::StringProcessCommands(char* theString)
{
    QString commandString(theString);
    QStringList commands = commandString.split(" ");
    bool returnvalue = ProcessCommands(commands);

    return returnvalue;
}

bool QOLSRApp::ProcessCommands(QStringList argv)
{
    for(int i=0; i < argv.length(); i++){
        if(argv[i] == "guiServerStart") {
            i++;
            if (argv[i] != serverPipeName) {
                pipeToServer.close();
                pipeToServer.connect(argv[i]);
                serverPipeName = argv[i];
                // TODO: send warning text to UI.
                qWarning() << "QOLSRApp::ProcessCommands(): Connecting to new server, "
                           << argv[i];
            } else {
                onGetSettings();
                onUpdateRoutes();
                onUpdateNeighbors();
            }
        } else if( argv[i]== "routes"){
            // nrlolsrd sends: routes [<destination> <gateway> <weight> <interface>]* end-routes
            // <weight> is a double.  The rest are strings.

            emit clearDestEntries();
            i++;
            for(; i+4< argv.length(); i+=4){
                emit addDestEntry(argv[i], argv[i+1], argv[i+2], argv[i+3]);
            }
        } else if(argv[i] == "neighbors") {
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
                emit addNeighborEntry(argv[i+ 0], argv[i+ 1], argv[i+ 2], argv[i+ 3]);
            }
        } else if(argv[i] == "settings"){
            // nrlolsrd sends: settings <all links> <fuzzy flooding> <tc slow down> \
            // <hello interval> <hello jitter> <hello timeout factor> \
            // <tc interval> <tc jitter> <tc timeout factor> \
            // <hna interval> <hna jitter> <hna timeout factor> \
            // <hysteresis up> <hysteresis down> <hysteresis alpha> <local willingness>
            //
            // <all links>, <fuzzy flooding>, <tc slow down>, and <local willingness> are ints. They represent bools.
            // The rest are doubles.
            i++;
            bool al = (argv[i+0].toInt() != 0);
            bool fuzzy = (argv[i+1].toInt() != 0);
            bool slowdown = (argv[i+2].toInt() != 0);
            bool willingness = (argv[i+15].toInt() != 0);

            emit setSettings(al, fuzzy, slowdown, //al fuzzy slowdown
                             argv[i+ 3].toDouble(), argv[i+ 4].toDouble(), argv[i+ 5].toDouble(), //hi hj ht
                             argv[i+ 6].toDouble(), argv[i+ 7].toDouble(), argv[i+ 8].toDouble(), //tci tcj tct
                             argv[i+ 9].toDouble(), argv[i+10].toDouble(), argv[i+11].toDouble(), //hnai hnaj hnat
                             argv[i+12].toDouble(), argv[i+13].toDouble(), argv[i+14].toDouble(), //up down alpha
                             willingness); //willingness
            i+=16;
        }
    }

    return true;
}

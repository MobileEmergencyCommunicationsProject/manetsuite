#include <QDebug>
#include <QDir>
#include "qolsrapp.h"
#include <QStringList>

QOLSRApp::QOLSRApp(): clientPipeName(QDir::tempPath() + "nrlolsr"),
    serverPipeName(QDir::tempPath() + "nrlolsrgui"),
    updateInterval(5000 /*milliseconds*/)
{
    updateTimer.setInterval(updateInterval);
    connect(&updateTimer, SIGNAL(timeout()),
            this, SLOT(onUpdateTimerTimeout()));

    /*
      PROBLEM! nrlolsrd uses unix domain sockets to communicate with
      the gui.  These sockets are of type SOCK_DGRAM.  SOCK_DGRAM is
      not supported by QLocalSocket.

      To talk to nrlolsrd, use socket(AF_UNIX, SOCK_DGRAM, 0).
      See ProtoPipe::Open() for help.
      */
    connect(&clientPipe, SIGNAL(connected()),
            this, SLOT(onClientConnected()));
    connect(&clientPipe, SIGNAL(disconnected()),
            this, SLOT(onClientDisconnected()));
    connect(&clientPipe, SIGNAL(error(QLocalSocket::LocalSocketError)),
            this, SLOT(onClientError(QLocalSocket::LocalSocketError)));
    connect(&clientPipe, SIGNAL(stateChanged(QLocalSocket::LocalSocketState)),
            this, SLOT(onClientStateChanged(QLocalSocket::LocalSocketState)));

    connect(&serverPipe, SIGNAL(newConnection()),
            this, SLOT(onServerNewConnection()));

    serverPipe.listen(serverPipeName); //pipe to receive messages
    clientPipe.connectToServer(clientPipeName, QIODevice::WriteOnly); //send pipe to nrlolsr
//    updateTimer.start();
}

QOLSRApp::~QOLSRApp()
{
    updateTimer.stop();
    serverPipe.close();
    clientPipe.disconnectFromServer();
}

void QOLSRApp::onClientConnected()
{
    //send the server pipe name to the client so it can connect back
    QString command("guiClientStart ");
    command.append(serverPipeName);
    QByteArray commandByteArray = command.toLocal8Bit();
    const char *commandBytes = commandByteArray.constData();

    if(clientPipe.write(commandBytes,strlen(commandBytes))){
        onGetSettings();
        onUpdateRoutes();
        onUpdateNeighbors();
    } else {
        // TODO: send critical text to the UI.
        qCritical() << "Could not send command \""
                    << command << "\" to server";
    }
}

void QOLSRApp::onClientDisconnected()
{
    // slot
    qCritical() << "Client " << clientPipeName << " disconnected";
}

void QOLSRApp::onClientError(QLocalSocket::LocalSocketError socketError)
{
    // slot
    qCritical() << "Client " << clientPipeName
                << " error " << socketError << " "
                << clientPipe.errorString();
}

void QOLSRApp::onClientStateChanged(QLocalSocket::LocalSocketState socketState)
{
    // slot
    qDebug() << "Client " << clientPipeName
             << " state " << socketState;
}


void QOLSRApp::onGetSettings()
{
    //send request to nrlolsr
      char buffer[256];
      memset((void*)buffer,0,255);
      buffer[255] = '\0';
      strcpy(buffer,"-sendGuiRoutes");
      unsigned int len=sizeof(buffer);

    if(QLocalSocket::ConnectedState == clientPipe.state()){
      if(len != clientPipe.write(buffer,len)){
        emit clearDestEntries();
        // TODO: send the critical text to the UI.
        qCritical() << "QOLSRApp::onGetSettings() client_pipe.write() error. Pipe name is "
                    << clientPipeName;
      }
    } else {
      emit clearDestEntries();
      // TODO: send the critical text to the UI
      qCritical() << "QOLSRApp::onGetSettings() client_pipe "
                  << clientPipeName
                  << " is not open! State is "
                  << clientPipe.state()
                  << " Cannot send request for update.";
    }
}

void QOLSRApp::onServerNewConnection()
{
    // slot
}

void QOLSRApp::onSettingsChanged(const bool al, const bool fuzzy, const bool slowdown,
                                 const double hi, const double hj, const double ht,
                                 const double tci, const double tcj, const double tct,
                                 const double hnai, const double hnaj, const double hnat,
                                 const double up, const double down, const double alpha,
                                 const int willingness)
{// willingness is ignored here because the original NRL OLSR GUI ignores it.
    QString command = QString("-al %1 -fuzzy %2 -slowdown %3 -hi %4 -hj %5 -ht %6 -tci %7 -tcj %8 -tct %9 -hnai %10 -hnaj %11 -hnat %12 -hys up %13 -hys down %14 -hys alpha %15").arg(al).arg(fuzzy).arg(slowdown).arg(hi).arg(hj).arg(ht).arg(tci).arg(tcj).arg(tct).arg(hnai).arg(hnaj).arg(hnat).arg(up).arg(down).arg(alpha);
}

void QOLSRApp::onUpdateNeighbors()
{
    //send request to nrlolsr
      char buffer[256];
      memset((void*)buffer,0,255);
      buffer[255] = '\0';
      strcpy(buffer,"-sendGuiNeighbors");
      unsigned int len=sizeof(buffer);

    if(QLocalSocket::ConnectedState == clientPipe.state()){
      if(len != clientPipe.write(buffer,len)){
        emit clearNeighborEntries();
        // TODO: send the critical text to the UI
        qCritical() << "QOLSRApp::onUpdateNeighbors() client_pipe.Send() error. Pipe name is "
                  << clientPipeName;
      }
    } else {
      emit clearNeighborEntries();
      // TODO: send the critical text to the UI
      qCritical() << "QOLSRApp::onUpdateNeighbors() client_pipe "
                  << clientPipeName
                  << " is not open! State is "
                  << clientPipe.state()
                  << " Cannot send request for update.";
    }
}

void QOLSRApp::onUpdateRoutes()
{
    //send request to nrlolsr
      char buffer[256];
      memset((void*)buffer,0,255);
      buffer[255] = '\0';
      strcpy(buffer,"-sendGuiRoutes");
      unsigned int len=sizeof(buffer);

    if(QLocalSocket::ConnectedState == clientPipe.state()){
      if(len != clientPipe.write(buffer,len)){
        emit clearDestEntries();
        // TODO: send critical text to the UI
        qCritical() << "QOLSRApp::onUpdateRoutes() client_pipe.Send() error. Pipe name is "
                    << clientPipeName;
      }
    } else {
      emit clearDestEntries();
      // TODO: send critical text to the UI
      qCritical() << "QOLSRApp::onUpdateRoutes() client_pipe "
                  << clientPipeName
                  << " is not open! State is "
                  << clientPipe.state()
                  << " Cannot send request for update.";
    }
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
    bool printusage = false;

    for(int i=0; i < argv.length(); i++){
        if(argv[i] == "guiServerStart") {
            i++;
            if (argv[i] != clientPipeName) {
                clientPipe.close();
                clientPipe.connectToServer(argv[i]);
                clientPipeName = argv[i];
                // TODO: send warning text to UI.
                qWarning() << "QOLSRApp::ProcessCommands(): Connecting to new server, "
                           << argv[i];
            } else {
                onGetSettings();
                onUpdateRoutes();
                onUpdateNeighbors();
            }
        } else if( argv[i]== "routes"){
            emit clearDestEntries();
            i++;
            for(; i+4< argv.length(); i+=4){
                emit addDestEntry(argv[i],argv[i+1],argv[i+2],argv[i+3]);
            }
        } else if(argv[i] == "neighbors") {
            emit clearNeighborEntries();
            i++;
            //add loop to add entries
            for(; i+4< argv.length(); i+=4){
                emit addNeighborEntry(argv[i+ 0],argv[i+ 1],argv[i+ 2],argv[i+ 3]);
            }
        } else if(argv[i] == "settings"){
            i++;
            bool al = (argv[i+0].toInt() != 0);
            bool fuzzy = (argv[i+1].toInt() != 0);
            bool slowdown = (argv[i+2].toInt() != 0);
            emit setSettings(al, fuzzy, slowdown,//al fuzzy slowdown
                             argv[i+ 3].toDouble(),argv[i+ 4].toDouble(),argv[i+ 5].toDouble(),//hi hj ht
                             argv[i+ 6].toDouble(),argv[i+ 7].toDouble(),argv[i+ 8].toDouble(),//tci tcj tct
                             argv[i+ 9].toDouble(),argv[i+10].toDouble(),argv[i+11].toDouble(),//hnai hnaj hnat
                             argv[i+12].toDouble(),argv[i+13].toDouble(),argv[i+14].toDouble(),//up down alpha
                             argv[i+15].toInt());                     //willingness
            i+=16;
        }
    }

    if(printusage){
        return false;
    }

    return true;
}
#if 0
void QOLSRApp::OnServerEvent(ProtoSocket&       /*theSocket*/,
                            ProtoSocket::Event theEvent)
{
  if (theEvent == ProtoSocket::RECV) {
    char buffer[8192];
    unsigned int len = 8191;
    if (serverPipe.Recv(buffer, len)){
      buffer[len]=0;
      if (len) {
          StringProcessCommands(buffer);
      }
    } else {
        qCritical() << "NrlOlsrGui::OnServerEvent: serverpipe.Recv() error";
    }
  }
}

void QOLSRApp::OnClientEvent(ProtoSocket&       /*theSocket*/,
                            ProtoSocket::Event theEvent)
{
    switch (theEvent)
    {
        case ProtoSocket::CONNECT:
             qDebug() << "QOLSRApp: client connected to server.");
             break;

        case ProtoSocket::RECV:
        {
            qDebug() << "QOLSRApp: client RECV event ...";
            break;
        }

        case ProtoSocket::SEND:
            qDebug() << "QOLSRApp: client SEND event ...";
            OnSendTimeout(updateTimer);
            break;

        case ProtoSocket::DISCONNECT:
            qDebug() << "QOLSRApp: client DISCONNECT event ...";
            clientPipe.Close();
            break;

        default:
            TRACE("QOLSRApp::OnClientEvent(%d) unhandled event type\n", theEvent);
            break;
    }  // end switch(theEvent)
}  // end PipeExample::OnClientEvent()
#endif


#include "olsrguitest.h"
#include <sys/time.h>

static const QString statusValues[] = {"LOST", "ASYM", "SYM", "MPR", "PENDING", "INVALID"};

OlsrGuiTest::OlsrGuiTest(QObject *parent) :
    QObject(parent), serverPipeName("/tmp/nrlolsrgui"), listeningPipeName("/tmp/nrlolsr")
{
    struct timeval time = {0 /* seconds */, 0 /* microseconds */};
    int success = gettimeofday(&time, 0);
    // BUG: should check success value.  0 means OK, -1 means error

    // seed the random number generator for
    // use by randomAddress & randomStatus()
    qsrand(time.tv_sec * 1000000 + time.tv_usec);

    _commandsForServer.clear();

    connect(&pipeFromClient, SIGNAL(readyRead()),
            this, SLOT(on_readyRead()));
    connect(&pipeToServer, SIGNAL(readyWrite()),
            this, SLOT(on_readyWrite()));
}

// Alternate between red and green
QString OlsrGuiTest::nextColor()
{
    QString answer = _colorValue ? "green" : "red";
    _colorValue = !_colorValue;
    return answer;
}

void OlsrGuiTest::on_readyRead()
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

void OlsrGuiTest::on_readyWrite()
{
    if (_commandsForServer.length() > 0) {
        QString first = _commandsForServer.first();
        qint64 bytesWritten = pipeToServer.write(first.toLocal8Bit());

        // TODO: If we couldn't write all of first,
        // consider removing the bytes we did write
        // and leave the rest of first at the head of
        // _commandsForServer
        if (bytesWritten == first.length()) {
            QString message = tr("sent command: %1");
            emit newMessage (message.arg(first), nextColor());
            _commandsForServer.removeFirst();
        } else {
            QString message = tr("short write: %1 instead of %2 (%3)");
            // BUG: This could put us in an infinite loop.
            // What if we can never write all of first?
            // Then we will never leave this short write condition.
            emit newMessage(message.arg(bytesWritten).arg(first.length()).arg(first), nextColor());
            pipeToServer.close();
            _commandsForServer.clear();
        }
    }
}

bool OlsrGuiTest::ProcessCommands(QStringList argv)
{
    for(int i=0; i < argv.length(); i++){

        if(argv.at(i) == "guiClientStart") {
            QString color = nextColor();
            QString message = "%1 %2";

            emit newMessage(message.arg(argv.at(i)).arg(argv.at(i+1)), color);

            i++;

            if (pipeToServer.isOpen()) {
                QString message = tr("Closing connecting to server, %1");
                emit newMessage(message.arg(serverPipeName), color);

                pipeToServer.close();
            }

            serverPipeName = argv.at(i);
            message = tr("Connecting to server, %1");
            emit newMessage(message.arg(serverPipeName), color);

            pipeToServer.connect(serverPipeName);


        } else if( argv.at(i)== "-sendGuiRoutes"){

            SendGuiRoutes();

        } else if(argv.at(i) == "-sendGuiNeighbors") {

            SendGuiNeighbors();

        } else if(argv.at(i) == "-sendGuiSettings"){

            SendGuiSettings();

        } else if ((argv.at(i) == "-al") || // on, off
                   (argv.at(i) == "-fuzzy") || // on, off
                   (argv.at(i) == "-hi") || // double
                   (argv.at(i) == "-hj") || // double
                   (argv.at(i) == "-hnai") || // double
                   (argv.at(i) == "-hnaj") || // double
                   (argv.at(i) == "-hnat") || // double
                   (argv.at(i) == "-ht") || // double
                   (argv.at(i) == "-slowdown") || // on, off
                   (argv.at(i) == "-tci") || // double
                   (argv.at(i) == "-tcj") || // double
                   (argv.at(i) == "-tct") || // double
                   (argv.at(i) == "-w")) // int
        {
            QString message = "%1 %2";
            emit newMessage(message.arg(argv.at(i)).arg(argv.at(i+1)), nextColor());
            i++;
        } else if (argv.at(i) == "-hys") {
            QString message = "%1 %2 %3";
            emit newMessage(message.arg(argv.at(i)).arg(argv.at(i+1)).arg(argv.at(i+2)), nextColor());
            i+=2;
        } else {
            QString message = tr("Unrecognized command from client: %1");
            emit newMessage(message.arg(argv.at(i)), nextColor());
        }
    }

    return true;
}

// Returns a random IPV4 address
QString OlsrGuiTest::randomAddress()
{
    QString answer(QString::number(qrand()%256));
    answer.append(".").append(QString::number(qrand()%256)).append(".").append(QString::number(qrand()%256)).append(".").append(QString::number(qrand()%256));

    return answer;
}

QString OlsrGuiTest::randomNeighbors(int numNeighbors)
{
    // nrlolsrd sends: neighbors [<address> <status> <connectivity> <mpr selector>]* end-neighbors
    // <address> is a string.
    // <status> is one of: LOST, ASYM, SYM, MPR, PENDING, INVALID
    // <connectivity> is a double
    // <mpr selector> is one of: TRUE, FALSE
    //
    // NRL's olsrgui calls <connectivity> "hysterisis".

    // XXX What is the correct range of values for connectivity?

    QString answer = "neighbors ";

    for (int i=0; i < numNeighbors; i++) {
        answer.append(randomAddress());
        answer.append(" ");
        answer.append(randomStatus());
        answer.append(" ");
        answer.append(QString::number(qrand()%10)); // connectivity or hysteresis
        answer.append (" ");
        answer.append((qrand()%2) ? "TRUE":"FALSE"); // mpr selector
        answer.append(" ");
    }

    answer.append("end-neighbors");

    return answer;
}

QString OlsrGuiTest::randomRoutes(int numRoutes)
{
    // nrlolsrd sends: routes [<destination> <gateway> <weight> <interface>]* end-routes
    // <weight> is a double.  The rest are strings.

    // XXX What is the correct range of values for weight?

    QString answer = "routes ";
    for (int i=0; i < numRoutes; i++) {
        answer.append(randomAddress());
        answer.append(" ");
        answer.append(randomAddress());
        answer.append(" ");
        answer.append(QString::number(qrand()%10)); // weight 0..9
        answer.append(" ");
        answer.append("wlan"); // wlan[0-3]
        answer.append(QString::number(qrand()%4));
        answer.append(" ");
    }

    answer.append("end-routes");

    return answer;
}

QString OlsrGuiTest::randomStatus()
{
    QString answer = statusValues[qrand()%6];
    return answer;
}

void OlsrGuiTest::SendGuiNeighbors()
{
    int numNeighbors = qrand()%30;
    QString neighbors = randomNeighbors(numNeighbors);
    emit newMessage(neighbors, nextColor());
    _commandsForServer.append(neighbors);
}

void OlsrGuiTest::SendGuiRoutes()
{
    int numRoutes = qrand()%30;
    QString routes = randomRoutes(numRoutes);
    emit newMessage(routes, nextColor());
    _commandsForServer.append(routes);
}

void OlsrGuiTest::SendGuiSettings()
{
    // nrlolsrd sends: settings <all links> <fuzzy flooding> <tc slow down> \
    // <hello interval> <hello jitter> <hello timeout factor> \
    // <tc interval> <tc jitter> <tc timeout factor> \
    // <hna interval> <hna jitter> <hna timeout factor> \
    // <hysteresis up> <hysteresis down> <hysteresis alpha> <local willingness>
    //
    // <all links>, <fuzzy flooding>, <tc slow down>, and <local willingness> are ints.
    // All but <local willingness> represent bools.
    // The rest are doubles.

    // XXX What are the correct ranges of values for these settings?

    QString settings = "settings ";
    settings.append(QString::number(qrand()%2)); // all links
    settings.append(" ");
    settings.append(QString::number(qrand()%2)); // fuzzy flooding
    settings.append(" ");
    settings.append(QString::number(qrand()%2)); // tc slow down
    settings.append(" ");
    settings.append(QString::number(((double)(qrand()%100000))/10000.0)); // hello interval
    settings.append(" ");
    settings.append(QString::number(((double)(qrand()%100000))/10000.0)); // hello jitter
    settings.append(" ");
    settings.append(QString::number(((double)(qrand()%100000))/10000.0)); // hello timeout factor
    settings.append(" ");
    settings.append(QString::number(((double)(qrand()%100000))/10000.0)); // tc interval
    settings.append(" ");
    settings.append(QString::number(((double)(qrand()%100000))/10000.0)); // tc jitter
    settings.append(" ");
    settings.append(QString::number(((double)(qrand()%100000))/10000.0)); // tc timeout factor
    settings.append(" ");
    settings.append(QString::number(((double)(qrand()%100000))/10000.0)); // hna interval
    settings.append(" ");
    settings.append(QString::number(((double)(qrand()%100000))/10000.0)); // hna jitter
    settings.append(" ");
    settings.append(QString::number(((double)(qrand()%100000))/10000.0)); // hna timeout factor
    settings.append(" ");
    settings.append(QString::number(((double)(qrand()%100000))/10000.0)); // hysteresis up
    settings.append(" ");
    settings.append(QString::number(((double)(qrand()%100000))/10000.0)); // hysteresis down
    settings.append(" ");
    settings.append(QString::number(((double)(qrand()%100000))/10000.0)); // hysteresis alpha
    settings.append(" ");
    settings.append(QString::number(qrand()%11)); // local willingness

    emit newMessage(settings, nextColor());
    _commandsForServer.append(settings);
}

void OlsrGuiTest::start()
{
    emit newMessage(tr("Started listening on %1").arg(listeningPipeName), nextColor());
    pipeFromClient.listen(listeningPipeName); //pipe to receive messages
}

void OlsrGuiTest::stop()
{
    emit newMessage(tr("Stopped listening on %1").arg(listeningPipeName), nextColor());

    if (pipeToServer.isOpen()) {
        pipeToServer.close();
    }

    if (pipeFromClient.isOpen()) {
        pipeFromClient.close();
    }

    _commandsForServer.clear();
}

bool OlsrGuiTest::StringProcessCommands(char* theString)
{
    QString commandString(theString);
    QStringList commands = commandString.split(" ");
    bool returnvalue = ProcessCommands(commands);

    return returnvalue;
}

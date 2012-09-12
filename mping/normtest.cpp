#include "normtest.h"
#include <QByteArray>
#include <QList>
#include <QString>
//#include <QDebug>

#define ADDR "224.1.2.3"
#define INTERFACE "wlan0"
#define PORT 6003

NormTest::NormTest(QObject *parent) :
    QObject(parent),
    _colorValue(false), _isReceiver(false), _readyWrite(false),
    _sequenceNumber(0), _stream(0), _timer(0)
{
}

NormTest::~NormTest()
{

}

// Alternate between red and green
QString NormTest::nextColor()
{
    QString answer = _colorValue ? "green" : "red";
    _colorValue = !_colorValue;
    return answer;
}

//
// Read all bytes from _stream
// Parse them into QStrings
// Raise the newMessage signal with each QString and the current color.
//
// The receiver of the signal, at its option, can display
// the QString in color to indicate
// that they were all received at the same time.
//
void NormTest::on_readyRead()
{
    QStringList messages;
    QByteArray bigBuf = _stream->readAll();
    if (bigBuf.length() > 0) {
        QString color = nextColor();
        QList<QByteArray>messages = bigBuf.split(0);

        qDebug() << "Read " << messages.size() << " QStrings";
        for (int i=0; i < messages.size(); i++) {
            if (i < messages.size() - 1) {
//                qDebug() << QString(messages.at(i)) << ", " << color;
                emit newMessage(QString(messages.at(i)), color);
            }
        }
    }
}

void NormTest::on_readyWrite()
{
    _readyWrite = true;
}

void NormTest::startReceiver()
{
    _isReceiver = true;
    _stream = new QNormTransportStream(ADDR, PORT, NORM_NODE_ANY);
    _stream->setMulticastInterface(INTERFACE);
    QObject::connect(_stream, SIGNAL(readyRead()), this, SLOT(on_readyRead()));
    if (_stream->open(QIODevice::ReadOnly)) {

        emit newMessage("Receiver started", nextColor());
    } else {
        emit newMessage("Receiver failed to start", nextColor());
    }
}

void NormTest::startSender()
{
    _isReceiver = false;
    _stream = new QNormTransportStream(ADDR, PORT, NORM_NODE_ANY);
    _stream->setMulticastInterface(INTERFACE);

    // signal existing receivers of this new session
    _stream->_senderParameters.sessionId++;

    QObject::connect(_stream, SIGNAL(readyWrite()), this, SLOT(on_readyWrite()));
    if (_stream->open(QIODevice::WriteOnly)) {
        _timer = new QTimer(this);
        connect(_timer, SIGNAL(timeout()), this, SLOT(update()));
        _timer->start(1000);

        emit newMessage("Sender started", nextColor());
    } else {
        emit newMessage("Sender failed to start", nextColor());
    }
}

void NormTest::stop()
{
    _stream->close();
    delete _stream;

    if (_timer) {
        _timer->stop();
        delete _timer;
    }

    if (_isReceiver) emit newMessage("Receiver stopped", nextColor());
    else emit newMessage("Sender stopped", nextColor());

    // Reset all of these to benign values
    // because this NormTest could be asked to
    // run itself again in a different mode.
    //
    // E.g. After running as the sender, this
    // NormTest has a non-zero _timer.  If run next
    // as a receiver, it still has non-zero _timer but
    // _timer points to deallocated memory, so telling
    // it stop() likely results in SIGSEGV.
    //
    _colorValue = false;
    _isReceiver = false;
    _readyWrite = false;
    _sequenceNumber = 0;
    _stream = 0;
    _timer = 0;
}

// BUG: The first message always has an extra newline.
void NormTest::update()
{
    qint64 numBytes;

    if (_readyWrite) {
        // BUG: Could write fewer than sizeof(qint64) bytes.
        // The next time on_readyWrite() is called,
        // it should write the remaining bytes.

        QString msg = QString("Message %1").arg(_sequenceNumber);
        _sequenceNumber ++;
        numBytes = _stream->write(msg.toAscii().constData(), msg.length() + 1);
        _readyWrite = ((msg.length()+ 1) == numBytes);

        emit newMessage(msg, nextColor());

        if (_readyWrite) {
//            _sender->markEom();
            _stream->flush(true); // mark end-of-message, then flush
        }
    }
}

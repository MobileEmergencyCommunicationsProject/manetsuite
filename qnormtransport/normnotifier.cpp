#include "normnotifier.h"
#include <QDebug>
#include "qnormtransport.h"

NormNotifier::NormNotifier(NormInstanceHandle normInstance, QObject *parent) :
    QSocketNotifier(NormGetDescriptor(normInstance), QSocketNotifier::Read, parent),
    _normInstance(normInstance)
{
    connect(this, SIGNAL(activated(int)), this, SLOT(notify(int)));
}

NormNotifier::~NormNotifier()
{
    _normInstance = NORM_INSTANCE_INVALID;
}

void NormNotifier::notify(int socket)
{
    NormEvent event;

    setEnabled(false);

    if (NormGetNextEvent(_normInstance, &event)) {
        // TODO: Learn about <reinterpret cast>?
        QNormTransport *t = (QNormTransport *) NormGetUserData(event.session);

        switch (event.type) {

        // NORM sender events
        case NORM_TX_QUEUE_VACANCY:
            t->normTxQueueVacancy(&event);
            break;
        case NORM_TX_QUEUE_EMPTY:
            t->normTxQueueEmpty(&event);
            break;
        case NORM_TX_FLUSH_COMPLETED:
            t->normTxFlushCompleted(&event);
            break;
        case NORM_TX_WATERMARK_COMPLETED:
            t->normTxWatermarkCompleted(&event);
            break;
        case NORM_TX_OBJECT_SENT:
            t->normTxObjectSent(&event);
            break;
        case NORM_TX_OBJECT_PURGED:
            t->normTxObjectPurged(&event);
            break;
        case NORM_TX_CMD_SENT:
            t->normTxCmdSent(&event);
            break;
        case NORM_TX_RATE_CHANGED:
            t->normTxRateChanged(&event);
            break;
        case NORM_LOCAL_SENDER_CLOSED:
            t->normLocalSenderClosed(&event);
            break;
        case NORM_CC_ACTIVE:
            t->normCCActive(&event);
            break;
        case NORM_CC_INACTIVE:
            t->normCCInactive(&event);
            break;

        // NORM receiver events
        case NORM_REMOTE_SENDER_NEW:
            t->normRemoteSenderNew(&event);
            break;
        case NORM_REMOTE_SENDER_ACTIVE:
            t->normRemoteSenderActive(&event);
            break;
        case NORM_REMOTE_SENDER_INACTIVE:
            t->normRemoteSenderInactive(&event);
            break;
        case NORM_REMOTE_SENDER_PURGED:
            t->normRemoteSenderPurged(&event);
            break;
        case NORM_RX_OBJECT_NEW:
//            qDebug() << "NormNotifier::notify(): NORM_RX_OBJECT_NEW";
            t->normRxObjectNew(&event);
            break;
        case NORM_RX_OBJECT_INFO:
            t->normRxObjectInfo(&event);
            break;
        case NORM_RX_OBJECT_UPDATED:
//            qDebug() << "NormNotifier::notify(): NORM_RX_OBJECT_UPDATED";
            t->normRxObjectUpdated(&event);
            break;
        case NORM_RX_OBJECT_COMPLETED:
//            qDebug() << "NormNotifier::notify(): NORM_RX_OBJECT_COMPLETED";
            t->normRxObjectCompleted(&event);
            break;
        case NORM_RX_OBJECT_ABORTED:
//            qDebug() << "NormNotifier::notify(): NORM_RX_OBJECT_ABORTED";
            t->normRxObjectAborted(&event);
            break;
        case NORM_RX_CMD_NEW:
            t->normRxCmdNew(&event);
            break;

        // NORM miscellaneous events
        case NORM_GRTT_UPDATED:
            t->normGrttUpdated(&event);
            break;
        case NORM_EVENT_INVALID:
            t->normEventInvalid(&event);
            break;

        default:
            // complain!
            qCritical("NormNotifier::notify(): unknown event: %d", event.type);
            break;
        }
    } /*else { // XXX Is it bad to ignore bogus notifications?
        // NormGetNextEvent returned false.
        // This might mean that the NormInstance has been stopped.
        // It might mean that the event is to be ignored.
        qDebug() << "NormNotifier::notify(): NormGetNextEvent() returned false";
    }*/

    setEnabled(true);
}

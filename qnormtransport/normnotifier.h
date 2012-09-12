#ifndef NORMNOTIFIER_H
#define NORMNOTIFIER_H

#include <normApi.h>
#include <QSocketNotifier>

/*
  NormNotifier dispatches NORM events. It is a helper class for QNormTransport.
  When it receives the Activated(int) signal, it reads one NormEvent from the
  NormInstance and dispatches the event to the corresponding QNormTransport.

  NormNotifiers are managed by NormInstanceManager. There is one NormNotifier
  for each NormInstanceHandle. These pairs are unique.
  */
class NormNotifier : public QSocketNotifier
{
    Q_OBJECT
public:
    explicit NormNotifier(NormInstanceHandle normInstance, QObject *parent = 0);
    ~NormNotifier();

signals:
    
public slots:
    // notify(int) is invoked whenever its Acitvate(int) signal is emitted.
    void notify(int socket);

private:
    NormInstanceHandle _normInstance;
};

#endif // NORMNOTIFIER_H

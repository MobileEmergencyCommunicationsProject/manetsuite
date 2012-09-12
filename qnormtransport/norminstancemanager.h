#ifndef NORMINSTANCEMANAGER_H
#define NORMINSTANCEMANAGER_H

#include <normApi.h>
#include "normnotifier.h"
#include <QHash>
#include <QSocketNotifier>

/*
  NormInstanceManager is a singleton that tracks the usage of pairs of
  <NormInstanceHandle, QSocketNotifier>. It deletes them when there are no
  users. It creates a default pair on demand. It is intended as a helper class
  for QNormTransport.
  */
class NormInstanceManager
{
public:
    // Returns the singleton NormInstanceManager
    static NormInstanceManager* instance();

    // Returns the default NormInstanceHandle. In order to use the default
    // NormInstanceHandle under NormInstanceManager's management, you must call
    // reserveNormInstanceHandle().
    NormInstanceHandle defaultNormInstanceHandle();

    // Call releaseNormInstance when you no longer need your
    // NormInstanceHandle. You must call this whether you use the default
    // NormInstanceHandle or your own NormInstanceHandle. This might result in
    // the destruction of a NormInstance.
    //
    // Returns true if normInstanceHandle is known to NormInstanceManager.
    // Returns fals if normInstanceHandle is now known to NormInstanceManager.
    bool releaseNormInstanceHandle(NormInstanceHandle normInstanceHandle);

    // Call reserveNormInstanceHandle() to indicate that you want to use a
    // particular NormInstanceHandle. reserveNormInstanceHandle() creates a
    // NormNotifer for this NormInstanceHandle unless you supply your own. If
    // you supply a NormNotifier, it replaces the existing one for
    // normInstanceHandle. The existing one is destroyed.
    void reserveNormInstanceHandle(NormInstanceHandle normInstanceHandle,
                                   NormNotifier *newNotifier = 0);

private:
    NormInstanceManager();
    ~NormInstanceManager();

    // Invoke when you no longer need the NormNotifier that corresponds to
    // normInstanceHandle
    void removeNormNotifier(NormInstanceHandle normInstanceHandle);

    NormInstanceHandle _defaultNormInstanceHandle;

    QHash<NormInstanceHandle, int> _instances;
    QHash<NormInstanceHandle, NormNotifier *> _notifiers;
};

#endif // NORMINSTANCEMANAGER_H

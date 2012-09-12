#include "norminstancemanager.h"

NormInstanceManager *_instance = 0;

NormInstanceManager::NormInstanceManager() :
    _defaultNormInstanceHandle(0)
{
}

NormInstanceHandle NormInstanceManager::defaultNormInstanceHandle()
{
    if (0 == _defaultNormInstanceHandle) {
        _defaultNormInstanceHandle = NormCreateInstance();
    }

    return _defaultNormInstanceHandle;
}

NormInstanceManager *NormInstanceManager::instance()
{
    if (0 == _instance) {
        _instance = new NormInstanceManager();
    }

    return _instance;
}


bool NormInstanceManager::releaseNormInstanceHandle(NormInstanceHandle normInstanceHandle)
{
    bool answer = false;
    if (_instances.contains(normInstanceHandle)) {
        answer = true;

        int count = _instances.value(normInstanceHandle);
        count --;
        if (count > 0) {
            _instances.insert(normInstanceHandle, count);
        } else {
            removeNormNotifier(normInstanceHandle);
            count = _instances.remove(normInstanceHandle);

            // Stop and destroy.
            NormStopInstance(normInstanceHandle);
            NormDestroyInstance(normInstanceHandle);

            if (normInstanceHandle == _defaultNormInstanceHandle)
                _defaultNormInstanceHandle = 0;
        }
    } // else: normInstanceHandle isn't managed by NormInstanceManager.

    return answer;
}

void NormInstanceManager::removeNormNotifier(NormInstanceHandle normInstanceHandle)
{
    NormNotifier *n = _notifiers.value(normInstanceHandle);

    if (0 != n) {
        _notifiers.remove(normInstanceHandle);

        delete n;
    }
}

void NormInstanceManager::reserveNormInstanceHandle(NormInstanceHandle normInstanceHandle,
                                                    NormNotifier *normNotifier)
{
    _instances.insert(normInstanceHandle,
                      1 + _instances.value(normInstanceHandle, 0));

    // Did the user supply a NormNotifier?
    // Y: Is there already one in use for normInstanceHandle?
    //    Y: removeNormNotifer(normInstanceHandle); insert normNotifier
    //    N: insert normNotifer
    // N: Is there already one in use for normInstanceHandle?
    //    Y: do nothing
    //    N: create a NormNotifier; insert the NormNotifier

    if (0 != normNotifier) {
        if (_notifiers.contains(normInstanceHandle)) {
            removeNormNotifier(normInstanceHandle);
        }

        _notifiers.insert(normInstanceHandle, normNotifier);
    } else {
        if (! _notifiers.contains(normInstanceHandle)) {
            NormNotifier *n = new NormNotifier(normInstanceHandle);

            _notifiers.insert(normInstanceHandle, n);
        }
    }
}

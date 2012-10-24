#include "pipetest.h"

PipeTest::PipeTest(QString pipeName, QObject *parent) :
    QObject(parent), _pipeName(pipeName) {
}

PipeTest::~PipeTest() {
    _pipe.close();
}

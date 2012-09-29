#ifndef QNORMFILETRANSPORTTEST_H
#define QNORMFILETRANSPORTTEST_H

#include "authority.h"
#include <QNormFileTransport>

class QNormFileTransportTest : public QNormFileTransport
{
    Q_OBJECT
public:
    explicit QNormFileTransportTest(const char* address, unsigned short port, NormNodeId normNodeId = NORM_NODE_ANY,
                           QObject *parent = 0);
    virtual ~QNormFileTransportTest();

    virtual void normRxObjectInfo(NormEvent *event);
    void setAuthority(Authority *authority);

private:
    // Ask _authority for permission to do things
    // like renaming a file.
    Authority *_authority;
};

#endif // QNORMFILETRANSPORTTEST_H

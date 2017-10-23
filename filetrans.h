#ifndef FILETRANS_H
#define FILETRANS_H

#include "myudpsocket.h"
#include "mytcpsocket.h"
class fileTrans
{
private:
    QString path;
    int window;
    unsigned long ip;
    unsigned port;
    myUdpSocket fileSocket;
    myTcpSocket headSocket;
public:
    fileTrans(QString path,unsigned long ip,unsigned port);
private:
    void fileInfo(QString path);
};

#endif // FILETRANS_H

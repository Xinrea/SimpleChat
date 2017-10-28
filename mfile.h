#ifndef MFILE_H
#define MFILE_H

#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <thread>
#include <QTime>
#include "myudpsocket.h"
#include "mytcpsocket.h"

class mfile:public QObject
{
    Q_OBJECT
private:
    WSADATA wsaData;
    sockaddr_in addr;
    SOCKET listenSocket = 0;
    sockaddr from;
    int fromlen;
    int errorCode;

    unsigned long f_ip;
    unsigned f_port;

    unsigned long tf_ip;
    unsigned tf_port;
    QString filepath;

    sockaddr_in remoteAddr;
    int nAddrLen;

    QString filename;
    unsigned filePackNumber = 0;
    unsigned filesize = 0;
    unsigned sendfilesize;

public:

public:
    mfile();
    void config(unsigned port,QString filename,unsigned filepack,unsigned filesize);
    void sendFile(QString path,unsigned account,unsigned long t_ip,unsigned t_port);

signals:
    void progressTo(int t,double s);

private:
    void readytoReceive();//单独线程
    void fileSendThread();
};

#endif // MFILE_H

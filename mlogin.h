#ifndef MLOGIN_H
#define MLOGIN_H

#include "myTcpSocket.h"
#include <thread>

#define UPDATETIME 10
#define CONNECTERROR 1
#define SENDERROR 2
#define RECVERROR 3


class mlogin
{
private:
    WCHAR serverIP[16];
    int serverPort;
    WCHAR configPath[25] = L".//config.ini";
public:
    unsigned account;
    unsigned session;
    unsigned port = 6330;
public:
    mlogin();
    unsigned login(unsigned accountid, char password[PASSLEN]);
    bool logout();
    unsigned getSession() const;
    ~mlogin();
};



#endif // MLOGIN_H

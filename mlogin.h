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
    bool updateFlag = false;
    unsigned session;
    unsigned account;
    unsigned port = 0;
    WCHAR serverIP[16];
    int serverPort;
    WCHAR configPath[25] = L".//config.ini";
public:
    mlogin();
    unsigned login(unsigned accountid, char password[PASSLEN]);
    unsigned getSession() const;
    bool logout();
    bool update();
    ~mlogin();
};



#endif // MLOGIN_H

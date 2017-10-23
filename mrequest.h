#ifndef MREQUEST_H
#define MREQUEST_H
#include "mytcpsocket.h"
#define ERROR_SOCKETCONNECT 1
#define ERROR_SOCKETSEND 2
#define ERROR_SOCKETRECV 3
#define ERROR_BLANK 4
#define ERROR_OFFLINE 5

class mrequest
{
private:
    WCHAR serverIP[16];
    int serverPort;
    WCHAR configPath[25] = L".//config.ini";
public:
    unsigned account;
    unsigned session;
public:
    mrequest();
    void config(unsigned accounts,unsigned sessions);
    unsigned getUserinfo(unsigned accountid, char username[8],unsigned long &ip,unsigned &port);
    ~mrequest();
};

#endif // MREQUEST_H

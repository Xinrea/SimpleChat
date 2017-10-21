#ifndef MFINDPWD_H
#define MFINDPWD_H


#include "myTcpSocket.h"
#include "msgstruct.h"
#define ERROR_SOCKETCONNECT 1
#define ERROR_SOCKETSEND 2
#define ERROR_SOCKETRECV 3
#define ERROR_CHANGEFAILE 4

class mfindpwd
{
private:
    unsigned account;
    WCHAR serverIP[16];
    int serverPort;
    WCHAR configPath[25] = L".//config.ini";
public:
    mfindpwd();
    unsigned findpwd(int accountid,const char comfirminfo[12], const char newpwd[PASSLEN]);
    ~mfindpwd();
};

#endif // MFINDPWD_H

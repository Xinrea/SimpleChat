#ifndef MREGISTER_H
#define MREGISTER_H


#include "myTcpSocket.h"
#define ERROR_SOCKETCONNECT 1
#define ERROR_SOCKETSEND 2
#define ERROR_SOCKETRECV 3
class mregister
{
private:
    WCHAR serverIP[16];
    int serverPort;
    WCHAR configPath[25] = L".//config.ini";
public:
    mregister();
    bool refresh();
    unsigned registerOnServer(const char username[8], const char password[PASSLEN], const char comfirminfo[12], unsigned& accountid);
    ~mregister();
};

#endif // MREGISTER_H

#ifndef LOCALSERVER_H
#define LOCALSERVER_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include "msgstruct.h"

class localServer
{
private:
    WSADATA wsaData;
    sockaddr_in addr;
    SOCKET mainSocket;
    int errorCode;
public:
    localServer();
    bool config(const unsigned port);
};

#endif // LOCALSERVER_H

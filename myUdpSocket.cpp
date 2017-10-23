#include "stdafx.h"
#include "myUdpSocket.h"

myUdpSocket::myUdpSocket() :errorCode(0)
{
    fromlen = sizeof(from);
}


myUdpSocket::~myUdpSocket()
{
}

bool myUdpSocket::config(WCHAR* ip, const int port)//config socket setting, ready to connect
{
    errorCode = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (errorCode != NO_ERROR) {

        return false;
    }
    connectSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (connectSocket == INVALID_SOCKET) {

        WSACleanup();
        return false;
    }
    char temp[16];
    WideCharToMultiByte(CP_ACP,0,ip,-1,temp,16,NULL,NULL);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(temp);
    addr.sin_port = htons(port);
    return true;
}

bool myUdpSocket::config(unsigned long ip, const int port)//config socket setting, ready to connect
{
    errorCode = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (errorCode != NO_ERROR) {

        return false;
    }
    connectSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (connectSocket == INVALID_SOCKET) {

        WSACleanup();
        return false;
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ip;
    addr.sin_port = htons(port);
    return true;
}


bool myUdpSocket::sendMsg(char* message, int length)//send message to the host
{
    errorCode = sendto(connectSocket,message,length,0,(sockaddr*)&addr,sizeof(addr));
    if (errorCode == SOCKET_ERROR)return false;
    return true;
}

bool myUdpSocket::recvMsg(char* message, int length)
{
    errorCode = recvfrom(connectSocket,message,length,0,&from,&fromlen);
    if (errorCode == SOCKET_ERROR)
    {
        return false;
    }
    return true;
}


bool myUdpSocket::clean()//disconnet for destorying or next config
{
    WSACleanup();
    return true;
}

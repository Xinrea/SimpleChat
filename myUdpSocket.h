#ifndef MYUDPSOCKET_H
#define MYUDPSOCKET_H


#include "msgstruct.h"
class myUdpSocket
{
private:
    WSADATA wsaData;
    sockaddr_in addr;
    SOCKET connectSocket;
    sockaddr from;
    int fromlen;
    int errorCode;
public:
    myUdpSocket();
    ~myUdpSocket();
    bool config(WCHAR* ip, const int port);//TODO:考虑一下port的类型
    bool sendMsg(char* message, int length = DATALEN);
    bool recvMsg(char* message, int length = DATALEN);//阻塞型，直至接收完毕
    bool clean();
    bool config(unsigned long ip, const int port);
};


#endif // MYUDPSOCKET_H

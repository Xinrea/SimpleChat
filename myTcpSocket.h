#pragma once
#include "stdafx.h"
#include "msgstruct.h"
class myTcpSocket
{
private:
	WSADATA wsaData;
	sockaddr_in addr;
	SOCKET connectSocket;
	int errorCode;
public:
	myTcpSocket();
	~myTcpSocket();
	bool config(WCHAR* ip, const int port);//TODO:考虑一下port的类型
	bool connectToHost();
	bool sendMsg(char* message, int length = DATALEN);
	bool recvMsg(char* message, int length = DATALEN);//阻塞型，直至接收完毕
	bool disconnect();
};


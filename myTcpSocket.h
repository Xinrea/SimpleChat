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
	bool config(WCHAR* ip, const int port);//TODO:����һ��port������
	bool connectToHost();
	bool sendMsg(char* message, int length = DATALEN);
	bool recvMsg(char* message, int length = DATALEN);//�����ͣ�ֱ���������
	bool disconnect();
};


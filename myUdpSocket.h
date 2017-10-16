#pragma once
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
	bool config(WCHAR* ip, const int port);//TODO:����һ��port������
	bool sendMsg(char* message, int length = DATALEN);
	bool recvMsg(char* message, int length = DATALEN);//�����ͣ�ֱ���������
	bool clean();
};


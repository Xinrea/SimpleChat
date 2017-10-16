#include "stdafx.h"
#include "myTcpSocket.h"


myTcpSocket::myTcpSocket():errorCode(0)
{
}


myTcpSocket::~myTcpSocket()
{
}

bool myTcpSocket::config(WCHAR* ip, const int port)//config socket setting, ready to connect
{
	errorCode = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (errorCode != NO_ERROR) {
		OutputDebugString(L"WSAStartup() Failed\n");
		return false;
	}
	connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connectSocket == INVALID_SOCKET) {
		OutputDebugString(L"socket() Failed\n");
		WSACleanup();
		return false;
	}
	addr.sin_family = AF_INET;
	InetPton(AF_INET, ip, &addr.sin_addr);
	addr.sin_port = htons(port);
	return true;
}

bool myTcpSocket::connectToHost()//connect to the host
{
	errorCode = connect(connectSocket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
	if (errorCode == SOCKET_ERROR) {
		OutputDebugString(L"connect() Failed\n");
		errorCode = closesocket(connectSocket);
		if (errorCode == SOCKET_ERROR) {
			OutputDebugString(L"closesocket() Failed\n");
			WSACleanup();
		}
		return false;
	}
	return true;
}

bool myTcpSocket::sendMsg(char* message, int length = DATALEN)//send message to the host
{
	errorCode = send(connectSocket, message,length, 0);
	if (errorCode == SOCKET_ERROR)return false;
	return true;
}

bool myTcpSocket::recvMsg(char* message, int length = DATALEN)
{
	errorCode = recv(connectSocket, message,length, MSG_WAITALL);
	if(errorCode == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}


bool myTcpSocket::disconnect()//disconnet for destorying or next config
{
	errorCode = closesocket(connectSocket);
	if (errorCode == SOCKET_ERROR) {
		OutputDebugString(L"closesocket() Failed\n");
		return false;
	}
	WSACleanup();
	return true;
}




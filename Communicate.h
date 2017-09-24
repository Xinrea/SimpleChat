#pragma once
#include "stdafx.h"
#define MAXBUF 100
class communicate
{
private:
	WSADATA wsa_data_;
	sockaddr_in client_service_;
	SOCKET connect_socket_;
	int result_;
public:
	communicate(WCHAR* ip, int port);
	~communicate();
	bool sconnect();
	bool sendmsg(char* message);
};


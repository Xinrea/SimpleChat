#pragma once
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
	unsigned registerOnServer(WCHAR username[8], char password[12], char comfirminfo[12], unsigned& accountid);
	~mregister();
};


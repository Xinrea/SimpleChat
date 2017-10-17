#pragma once
#include "myTcpSocket.h"
#include "msgstruct.h"
class mfindpwd
{
private:
	unsigned account;
	WCHAR serverIP[16];
	int serverPort;
	WCHAR configPath[25] = L".//config.ini";
public:
	mfindpwd();
	bool findpwd(int accountid,char comfirminfo[12]);
	~mfindpwd();
};


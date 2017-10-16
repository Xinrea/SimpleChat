#pragma once
#include "myTcpSocket.h"
#include <thread>
class mlogin
{
private:
	unsigned session;
	unsigned account;
	WCHAR serverIP[16];
	int serverPort;
	WCHAR configPath[25] = L".//config.ini";
public:
	mlogin(unsigned accountid, char password[12]);
	unsigned getSession() const;
	bool logout();
	bool update();
	~mlogin();
};


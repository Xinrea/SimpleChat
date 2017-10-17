#pragma once
#include "myTcpSocket.h"
#include <thread>

#define UPDATETIME 10
class mlogin
{
private:
	bool updateFlag = false;
	unsigned session;
	unsigned account;
	WCHAR serverIP[16];
	int serverPort;
	WCHAR configPath[25] = L".//config.ini";
public:
	mlogin();
	bool login(unsigned accountid, char password[12]);
	unsigned getSession() const;
	bool logout();
	bool update();
	~mlogin();
};


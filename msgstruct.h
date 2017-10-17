#pragma once
#include "stdafx.h"

#define DATALEN 128

struct stateMessage
{
	unsigned session;
	unsigned accountID;
	bool keepAlive;
};

struct basicMessage//12
{
	unsigned int session;
	unsigned int accountID;
	char body[116];
};

struct respondMessage
{
	unsigned int respondCode;
	unsigned int ipLen;
	char body[64];
};

struct loginMessage//24
{
	unsigned int session;
	unsigned int accountID;
	char password[12];
};

struct comfirmMessage//16
{
	unsigned int session;
	unsigned int accountID;
	unsigned short comfirmCode;
};

struct registerMessage//52
{
	unsigned int session;
	unsigned int accountID;
	WCHAR userName[8];
	char password[12];
	char comfirmInfo[12];
};

struct findpwdMessage//24
{
	unsigned int accountID;
	char comfirmInfo[12];
};

struct requestMessage//16
{
	unsigned int session;
	unsigned int accountID;
	unsigned int requestID;
};

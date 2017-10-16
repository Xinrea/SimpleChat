#pragma once
#include "stdafx.h"

#define DATALEN 128
#define BMSG 0
#define CMSG 1
#define RMSG 2
#define FMSG 3
#define CHMSG 4
#define RQMSG 5
#define LGMSG 6

struct stateMessage
{
	unsigned session;
	unsigned accountID;
	bool keepAlive;
};

struct basicMessage//12
{
	unsigned short msgType;
	unsigned int session;
	unsigned int accountID;
	char body[116];
};

struct respondMessage
{
	unsigned short msgType;
	unsigned int respondCode;
	unsigned int ipLen;
	char body[64];
	char pad[52];
};

struct loginMessage//24
{
	unsigned short msgType;
	unsigned int session;
	unsigned int accountID;
	char password[12];
	char pad[104];
};

struct comfirmMessage//16
{
	unsigned short msgType;
	unsigned int session;
	unsigned int accountID;
	unsigned short comfirmCode;
	char pad[112];
};

struct registerMessage//52
{
	unsigned short msgType;
	unsigned int session;
	unsigned int accountID;
	WCHAR userName[8];
	char password[12];
	char comfirmInfo[12];
	char pad[76];
};

struct findpwdMessage//24
{
	unsigned short msgType;
	unsigned int accountID;
	char comfirmInfo[12];
	char pad[108];
};

struct requestMessage//16
{
	unsigned short msgType;
	unsigned int session;
	unsigned int accountID;
	unsigned int requestID;
	char pad[112];
};

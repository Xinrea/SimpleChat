﻿#ifndef MSGSTRUCT_H
#define MSGSTRUCT_H

#include "stdafx.h"

#define DATALEN 128
#define PASSLEN 12
#define STATE 0
#define BASIC 1
#define RESPOND 2
#define LOGIN 3
#define COMFIRM 4
#define REGISTER 5
#define FINDPWD 6
#define REQUEST 7
#define FILEREQ 8

#define UDPLEN 1016

struct stateMessage
{
    unsigned msgType;
    unsigned session;
    unsigned accountID;
    bool keepAlive;
    char pad[128-15];
};

struct basicMessage//12
{
    unsigned msgType;//FILEREQ
    unsigned int session;//SIZE
    unsigned int accountID;//ACCOUNT
    unsigned targetID;//TOTAL
    char body[112];//FILENAME
};

struct respondMessage
{
    unsigned msgType;
    char username[8];
    unsigned long ip;
    unsigned port;
    char pad[112-sizeof(unsigned long)];
};

struct loginMessage//24
{
    unsigned msgType;
    unsigned int session;
    unsigned int accountID;
    unsigned port;
    char password[PASSLEN];
    char pad[104];
};

struct comfirmMessage//16
{
    unsigned msgType;
    unsigned int session;
    unsigned int accountID;
    unsigned short comfirmCode;
    char pad[112];
};

struct registerMessage//52
{
    unsigned msgType;
    unsigned int session;
    unsigned int accountID;
    char userName[8];
    char password[PASSLEN];
    char comfirmInfo[12];
    char pad[84];
};

struct findpwdMessage//24
{
    unsigned msgType;
    unsigned int accountID;
    char password[PASSLEN];
    char comfirmInfo[12];
    char pad[96];
};

struct requestMessage//16
{
    unsigned msgType;
    unsigned int session;
    unsigned int accountID;
    unsigned int requestID;
    char pad[112];
};


struct fileblock
{
    unsigned number;//4
    unsigned datalen;
    char body[UDPLEN];
};

struct fileAck
{
    unsigned number;
};


#endif // MSGSTRUCT_H

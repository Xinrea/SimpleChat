#include "stdafx.h"
#include "mregister.h"


mregister::mregister()
{
    GetPrivateProfileString(L"SERVER", L"IP", L"127.0.0.1", serverIP, 16, configPath);
    serverPort = GetPrivateProfileInt(L"SERVER", L"PORT", 12000, configPath);
    qDebug("Port: %d\n",serverPort);
}


mregister::~mregister()
{
}

bool mregister::refresh()
{
    if(GetPrivateProfileString(L"SERVER", L"IP", L"127.0.0.1", serverIP, 16, configPath))
    {
        serverPort = GetPrivateProfileInt(L"SERVER", L"PORT", 0, configPath);

        return true;
    }
    return false;
}

unsigned mregister::registerOnServer(const char username[8], const char password[PASSLEN], const char comfirminfo[12], unsigned& accountid)
{
    registerMessage regMessage;//注册消息
    basicMessage recvMessage;//响应消息
    regMessage.msgType = REGISTER;
    regMessage.accountID = 0;
    regMessage.session = 0;
    strcpy_s(regMessage.userName,8,username);
    strcpy_s(regMessage.password, PASSLEN, password);
    strcpy_s(regMessage.comfirmInfo, 12, comfirminfo);
    myTcpSocket registerSocket;
    registerSocket.config(serverIP, serverPort);
    if(registerSocket.connectToHost())
    {
        if(registerSocket.sendMsg(reinterpret_cast<char*>(&regMessage)))
        {
            if(registerSocket.recvMsg(reinterpret_cast<char*>(&recvMessage)))
            {
                accountid = recvMessage.accountID;//得到注册账号
                registerSocket.disconnect();//断开连接
            }
            else return ERROR_SOCKETRECV;
        }
        else return ERROR_SOCKETSEND;
    }
    else return ERROR_SOCKETCONNECT;
    return 0;
}

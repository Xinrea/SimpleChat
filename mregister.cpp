#include "stdafx.h"
#include "mregister.h"


mregister::mregister()
{
	GetPrivateProfileString(L"SERVER", L"IP", L"127.0.0.1", serverIP, 16, configPath);
	serverPort = GetPrivateProfileInt(L"SERVER", L"PORT", 0, configPath);
	WCHAR debugInfo[30];
	wsprintf(debugInfo, L"%s || %d\n", serverIP, serverPort);
	OutputDebugString(debugInfo);
}


mregister::~mregister()
{
}

bool mregister::refresh()
{
	if(GetPrivateProfileString(L"SERVER", L"IP", L"127.0.0.1", serverIP, 16, configPath))
	{
		serverPort = GetPrivateProfileInt(L"SERVER", L"PORT", 0, configPath);
		OutputDebugString(L"ERROR: Can't read config.ini");
		return true;
	}
	return false;
}

unsigned mregister::registerOnServer(WCHAR username[8], char password[12], char comfirminfo[12], unsigned& accountid)
{
	registerMessage regMessage;//注册消息
	basicMessage recvMessage;//响应消息
	regMessage.msgType = RMSG;
	regMessage.accountID = 0;
	regMessage.session = 0;
	wcscpy_s(regMessage.userName,8,username);
	strcpy_s(regMessage.password, 12, password);
	strcpy_s(regMessage.comfirmInfo, 12, comfirminfo);
	myTcpSocket registerSocket;
	registerSocket.config(serverIP, serverPort);
	if(registerSocket.connectToHost())
	{
		if(registerSocket.sendMsg((char*)&regMessage))
		{
			if(registerSocket.recvMsg((char*)&recvMessage))
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


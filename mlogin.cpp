#include "stdafx.h"
#include "mlogin.h"


mlogin::mlogin(unsigned accountid, char password[12])
{
	account = accountid;
	//��ȡ������Ϣ
	GetPrivateProfileString(L"SERVER", L"IP", L"127.0.0.1", serverIP, 16, configPath);
	serverPort = GetPrivateProfileInt(L"SERVER", L"PORT", 0, configPath);
	WCHAR debugInfo[30];
	wsprintf(debugInfo, L"Login: %s || %d\n", serverIP, serverPort);
	OutputDebugString(debugInfo);
	//������Ϣ�ṹ
	loginMessage loginMsg;
	basicMessage recvMsg;
	loginMsg.msgType = 6;
	loginMsg.accountID = accountid;
	loginMsg.session = 0;
	strcpy_s(loginMsg.password,12,password);
	//���ͽ�����Ϣ
	myTcpSocket loginSocket;
	loginSocket.config(serverIP, serverPort);
	loginSocket.connectToHost();
	loginSocket.sendMsg((char*)&loginMsg);
	loginSocket.recvMsg((char*)&recvMsg);
	session = recvMsg.session;
}

unsigned mlogin::getSession() const
{
	return session;
}

bool mlogin::logout()
{
	stateMessage stateMsg, recvMsg;
	stateMsg.session = session;
	stateMsg.accountID = account;
	stateMsg.keepAlive = false;
	myTcpSocket logoutSocket;
	logoutSocket.config(serverIP, serverPort);
	logoutSocket.connectToHost();
	logoutSocket.sendMsg((char*)&stateMsg, sizeof(stateMessage));
	logoutSocket.sendMsg((char*)&recvMsg, sizeof(stateMessage));
	if(!recvMsg.keepAlive)
	{
		logoutSocket.disconnect();
		return true;
	}
	return false;
}

bool mlogin::update()
{
	//update online state
}


mlogin::~mlogin()
{
}

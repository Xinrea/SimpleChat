#include "stdafx.h"
#include "mlogin.h"

mlogin::mlogin()
{
	account = 0;
	session = 0;
	//��ȡ������Ϣ
	GetPrivateProfileString(L"SERVER", L"IP", L"127.0.0.1", serverIP, 16, configPath);
	serverPort = GetPrivateProfileInt(L"SERVER", L"PORT", 0, configPath);
}


bool mlogin::login(unsigned accountid, char password[12])
{
	account = accountid;
	//������Ϣ�ṹ
	loginMessage loginMsg;
	basicMessage recvMsg;
	loginMsg.accountID = accountid;
	loginMsg.session = 0;
	strcpy_s(loginMsg.password,12,password);
	//���ͽ�����Ϣ
	myTcpSocket loginSocket;
	loginSocket.config(serverIP, serverPort);
	OutputDebugString(L"log:connect\n");
	if (!loginSocket.connectToHost())return false;
	OutputDebugString(L"log:send\n");
	if (!loginSocket.sendMsg((char*)&loginMsg))return false;
	OutputDebugString(L"log:recv\n");
	loginSocket.recvMsg((char*)&recvMsg);
	session = recvMsg.session;
	if(session != 0)
	{
		updateFlag = true;
		OutputDebugString(L"log:Create thread\n");
		std::thread tupdate(&mlogin::update, this);
		tupdate.detach();//���߳��Լ�ִ��
		return true;
	}
	return false;
	
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
		updateFlag = false;
		return true;
	}
	return false;
}

bool mlogin::update()
{
	//update online state
	while(updateFlag)
	{
		Sleep(UPDATETIME * 1000);//ÿ����UPDATETIME�������ȷ������
		stateMessage stateMsg, recvMsg;
		stateMsg.session = session;
		stateMsg.accountID = account;
		stateMsg.keepAlive = true;
		myTcpSocket logoutSocket;
		logoutSocket.config(serverIP, serverPort);
		logoutSocket.connectToHost();
		logoutSocket.sendMsg((char*)&stateMsg, sizeof(stateMessage));
		logoutSocket.sendMsg((char*)&recvMsg, sizeof(stateMessage));
		session = recvMsg.session;
		logoutSocket.disconnect();
	}
	
	return true;
}


mlogin::~mlogin()
{
	updateFlag = false;
}

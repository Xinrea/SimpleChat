#include "stdafx.h"
#include "mfindpwd.h"

mfindpwd::mfindpwd()
{
	//∂¡»°≈‰÷√–≈œ¢
	GetPrivateProfileString(L"SERVER", L"IP", L"127.0.0.1", serverIP, 16, configPath);
	serverPort = GetPrivateProfileInt(L"SERVER", L"PORT", 0, configPath);
}


bool mfindpwd::findpwd(int accountid,char comfirminfo[12])
{
	findpwdMessage fpwdMsg;
	findpwdMessage recvMsg;
	fpwdMsg.accountID = accountid;
	strcpy_s(fpwdMsg.comfirmInfo, 12, comfirminfo);

	myTcpSocket fpwdSocket;
	fpwdSocket.config(serverIP, serverPort);
	fpwdSocket.connectToHost();
	fpwdSocket.sendMsg((char*)&fpwdMsg, sizeof(findpwdMessage));
	fpwdSocket.recvMsg((char*)&recvMsg, sizeof(findpwdMessage));
	fpwdSocket.disconnect();
	if (recvMsg.accountID)return true;
	return false;
}


mfindpwd::~mfindpwd()
{
}

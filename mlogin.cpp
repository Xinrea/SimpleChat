#include "stdafx.h"
#include "mlogin.h"

mlogin::mlogin()
{
    account = 0;
    session = 0;
    //读取配置信息
    GetPrivateProfileString(L"SERVER", L"IP", L"127.0.0.1", serverIP, 16, configPath);
    serverPort = GetPrivateProfileInt(L"SERVER", L"PORT", 12000, configPath);//有点问题
}


unsigned mlogin::login(unsigned accountid, char password[PASSLEN])
{
    account = accountid;
    //构建消息结构
    loginMessage loginMsg;
    basicMessage recvMsg;
    loginMsg.msgType = LOGIN;
    loginMsg.accountID = accountid;
    loginMsg.session = 0;
    loginMsg.port = port;
    strcpy_s(loginMsg.password,PASSLEN,password);
    //发送接收消息
    myTcpSocket loginSocket;
    qDebug("Port:%d\n",serverPort);
    loginSocket.config(serverIP, serverPort);

    if (!loginSocket.connectToHost())return CONNECTERROR;

    if (!loginSocket.sendMsg((char*)&loginMsg))return SENDERROR;

    if(!loginSocket.recvMsg((char*)&recvMsg))return RECVERROR;
    session = recvMsg.session;
    qDebug("GetSession:%d\n",session);
    if(session != 0)
    {
        qDebug("Login successfully! Start to update state\n");
        return 0;
    }
    return RECVERROR;

}

unsigned mlogin::getSession() const
{
    return session;
}

bool mlogin::logout()
{
    stateMessage stateMsg, recvMsg;
    stateMsg.msgType = STATE;
    stateMsg.session = session;
    stateMsg.accountID = account;
    stateMsg.keepAlive = false;
    myTcpSocket logoutSocket;
    logoutSocket.config(serverIP, serverPort);
    logoutSocket.connectToHost();
    logoutSocket.sendMsg(reinterpret_cast<char*>(&stateMsg));
    logoutSocket.recvMsg(reinterpret_cast<char*>(&recvMsg));
    if(!recvMsg.keepAlive)
    {
        logoutSocket.disconnect();
        return true;
    }
    return false;
}

mlogin::~mlogin()
{
}

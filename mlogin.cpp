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
        updateFlag = true;
        qDebug("Login successfully! Start to update state\n");
        std::thread tupdate(&mlogin::update, this);
        tupdate.detach();//子线程自己执行
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
        updateFlag = false;
        return true;
    }
    return false;
}

bool mlogin::update()
{
    WCHAR serverIP_[16];
    int serverPort_;
    serverPort_ = serverPort;
    wcscpy(serverIP_,serverIP);
    //update online state
    while(updateFlag)
    {
        Sleep(UPDATETIME * 1000);//每经过UPDATETIME向服务器确认在线
        stateMessage stateMsg, recvMsg;
        stateMsg.msgType = STATE;
        stateMsg.session = session;
        stateMsg.accountID = account;
        stateMsg.keepAlive = true;
        myTcpSocket logoutSocket;
        logoutSocket.config(serverIP_, serverPort_);
        logoutSocket.connectToHost();
        logoutSocket.sendMsg(reinterpret_cast<char*>(&stateMsg));
        logoutSocket.recvMsg(reinterpret_cast<char*>(&recvMsg));
        session = recvMsg.session;
        logoutSocket.disconnect();
    }

    return true;
}


mlogin::~mlogin()
{
    updateFlag = false;
}

#include "mrequest.h"

mrequest::mrequest()
{
    GetPrivateProfileString(L"SERVER", L"IP", L"127.0.0.1", serverIP, 16, configPath);
    serverPort = GetPrivateProfileInt(L"SERVER", L"PORT", 12000, configPath);
    qDebug("Port: %d\n",serverPort);
}

mrequest::~mrequest()
{

}

void mrequest::config(unsigned accounts, unsigned sessions)
{
    account = accounts;
    session = sessions;
}

unsigned mrequest::getUserinfo(unsigned accountid, char username[],unsigned long &ip,unsigned &port)
{
    requestMessage regMessage;//注册消息
    respondMessage recvMessage;//响应消息
    regMessage.msgType = REQUEST;
    regMessage.accountID = account;
    regMessage.session = session;
    regMessage.requestID = accountid;
    myTcpSocket requestSocket;
    requestSocket.config(serverIP, serverPort);
    if(requestSocket.connectToHost())
    {
        if(requestSocket.sendMsg(reinterpret_cast<char*>(&regMessage)))
        {
            if(requestSocket.recvMsg(reinterpret_cast<char*>(&recvMessage),sizeof(respondMessage)))
            {
                //得到指定帐号信息
                if(recvMessage.msgType == 1)
                {
                    strcpy(username,recvMessage.username);
                    ip = recvMessage.ip;
                    port = recvMessage.port;
                    requestSocket.disconnect();//断开连接
                    return 0;
                }
                else if(recvMessage.msgType == 2)
                {
                    strcpy(username,recvMessage.username);
                    requestSocket.disconnect();//断开连接
                    return ERROR_OFFLINE;
                }
                else
                {
                    requestSocket.disconnect();//断开连接
                    return ERROR_BLANK;
                }

            }
            else return ERROR_SOCKETRECV;
        }
        else return ERROR_SOCKETSEND;
    }
    else return ERROR_SOCKETCONNECT;
}

#include "stdafx.h"
#include "mfindpwd.h"

mfindpwd::mfindpwd()
{
    //读取配置信息
    GetPrivateProfileString(L"SERVER", L"IP", L"127.0.0.1", serverIP, 16, configPath);
    serverPort = GetPrivateProfileInt(L"SERVER", L"PORT", 12000, configPath);
}


unsigned mfindpwd::findpwd(int accountid,const char comfirminfo[12], const char newpwd[PASSLEN])
{
    findpwdMessage fpwdMsg;
    basicMessage recvMsg;
    fpwdMsg.msgType = FINDPWD;
    fpwdMsg.accountID = accountid;
    strcpy_s(fpwdMsg.comfirmInfo, 12, comfirminfo);
    strcpy_s(fpwdMsg.password, PASSLEN, newpwd);
    myTcpSocket fpwdSocket;
    fpwdSocket.config(serverIP, serverPort);

    if(fpwdSocket.connectToHost())
    {
        if(fpwdSocket.sendMsg(reinterpret_cast<char*>(&fpwdMsg)))
        {
            if(fpwdSocket.recvMsg(reinterpret_cast<char*>(&recvMsg)))
            {
                if (!recvMsg.accountID)//服务器返回0表示修改成功
                {
                    fpwdSocket.disconnect();//断开连接
                    return 0;
                }
                else return ERROR_CHANGEFAILE;
            }
            else return ERROR_SOCKETRECV;
        }
        else return ERROR_SOCKETSEND;
    }
    else return ERROR_SOCKETCONNECT;
}


mfindpwd::~mfindpwd()
{
}

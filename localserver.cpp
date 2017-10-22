#include "localserver.h"

localServer::localServer()
{

}

bool localServer::config(const unsigned port)//config socket setting, ready to connect
{
    errorCode = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (errorCode != NO_ERROR) {

        return false;
    }
    mainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (mainSocket == INVALID_SOCKET) {

        WSACleanup();
        return false;
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.S_un.S_addr = INADDR_ANY;//任意地址
    addr.sin_port = htons(port);
    return true;
}

bool localServer::startListen()//connect to the host
{
    qDebug("Log: Bind Port");
    errorCode = bind(mainSocket, (sockaddr*)&addr, sizeof(addr));
    if (errorCode != 0)
    {
        qDebug("Log: Bind failed!");
        return false;
    }
    qDebug("Log: Listen Port");
    errorCode = listen(mainSocket,10);
    if (errorCode != 0)
    {
        qDebug("Log: Listen failed!");
        return false;
    }
    while(true)
    {
        sockaddr_in clientAddr;
        int length = sizeof(clientAddr);
        SOCKET clientSocket = accept(mainSocket, (sockaddr*)&clientAddr,&length);
        if(clientSocket == INVALID_SOCKET)
        {
            qDebug("Accept failed");
        }
        else
        {
            qDebug("Log: Accept Client");
            char buff[128];
            errorCode = recv(clientSocket,buff,128,0);
            if (errorCode == SOCKET_ERROR)
            {
                qDebug("Recv failed");
            }
            else
            {
                char rebuff[128];
                if(respond(buff,clientAddr,rebuff))
                {
                    errorCode = send(clientSocket,rebuff,128,0);
                    if (errorCode == SOCKET_ERROR)
                    {
                        qDebug("Send failed");
                    }
                }
                else
                {
                    qDebug("Construct message failed");
                }
            }
        }


    }
}

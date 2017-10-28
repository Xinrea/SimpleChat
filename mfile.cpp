#include "mfile.h"

mfile::mfile()
{

}


void mfile::config(unsigned port,QString filename,unsigned filepack,unsigned filesize)
{
    this->filename = filename;
    this->filePackNumber = filepack;
    this->filesize = filesize;
    if(listenSocket != 0)return;
    errorCode = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (errorCode != NO_ERROR) {

        return;
    }
    listenSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (listenSocket == INVALID_SOCKET) {

        WSACleanup();
        return;
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    bind(listenSocket,(sockaddr*)&addr,sizeof(addr));
    std::thread recieveThread(&mfile::readytoReceive,this);
    recieveThread.detach();
}

void mfile::sendFile(QString path, unsigned account,unsigned long t_ip, unsigned t_port)
{
    basicMessage fileHead;
    basicMessage respond;
    QFileInfo fileinfo(path);
    fileHead.msgType = FILEREQ;
    fileHead.session = fileinfo.size();
    fileHead.accountID = account;
    fileHead.targetID = fileinfo.size()/UDPLEN+1;
    strcpy(fileHead.body,fileinfo.fileName().toStdString().data());
    myTcpSocket fileheadSocket;
    fileheadSocket.config(t_ip,t_port);
    fileheadSocket.connectToHost();
    fileheadSocket.sendMsg(reinterpret_cast<char*>(&fileHead),sizeof(basicMessage));
    fileheadSocket.recvMsg(reinterpret_cast<char*>(&respond),sizeof(basicMessage));
    if(!respond.msgType)return;
    fileheadSocket.disconnect();
    tf_ip = t_ip;
    tf_port = t_port+1;
    filepath = path;
    sendfilesize = fileinfo.size();
    qDebug("Create filesend thread");
    std::thread sendThread(&mfile::fileSendThread,this);
    sendThread.detach();

}

void mfile::fileSendThread()
{
    myUdpSocket dataSocket;
    QFile file(filepath);
    if(!file.open(QFile::ReadOnly))
    {
        qDebug("Open File Failed");
        return;
    }
    qDebug("Open File");
    char* buff = new char[sendfilesize];//内存空间
    char* buff_s = buff;
    unsigned readlen = file.read(buff,sendfilesize);//全部读入到内存
    if(readlen!=sendfilesize){
        qDebug("Read Error");
        return;
    }
    file.close();
    dataSocket.config(tf_ip,tf_port);
    unsigned leftlen = sendfilesize,number = 1;
    bool stop = false;
    while(true)
    {
        fileblock block;
        fileAck ack;
        if(leftlen <= UDPLEN)
        {
            //不够整个包
            qDebug("尾包出现");
            memcpy(block.body,buff_s,leftlen);
            block.datalen = leftlen;
            stop = true;
        }
        else
        {
            //填充整个数据空间
            qDebug("全包: %d",number);
            memcpy(block.body,buff_s,UDPLEN);
            buff_s +=UDPLEN;//指针移动
            leftlen = leftlen - UDPLEN;
            block.datalen = UDPLEN;
        }

        block.number = number;
        do
        {
           dataSocket.sendMsg(reinterpret_cast<char*>(&block),sizeof(fileblock));
        }
        while(!dataSocket.recvMsg(reinterpret_cast<char*>(&ack),sizeof(fileAck)));
        //qDebug("Send %d",number);
        //qDebug("Recieve Ack：%d",ack.number);
        while(true)//重发
        {
            if(ack.number != number)
            {
                //Sleep(100);
                qDebug("ACK不对，重发（记录：%d | 收到：%d）",number,ack.number);
                do
                {
                   dataSocket.sendMsg(reinterpret_cast<char*>(&block),sizeof(fileblock));
                }
                while(!dataSocket.recvMsg(reinterpret_cast<char*>(&ack),sizeof(fileAck)));
            }
            else break;
        }
        number++;
        if(stop)break;
    }
    qDebug("发送结束");
    dataSocket.clean();
    delete []buff;
    buff = nullptr;
}

void mfile::readytoReceive()
{
    unsigned filePackNumber_ = filePackNumber;
    unsigned recievedNumber = 0;
    char* buffer = new char[filesize];
    char* buffer_s = buffer;
    QTime time1;
    time1.start();
    while(true)
    {
        fileblock buff;
        nAddrLen = sizeof(sockaddr_in);
        int ret = recvfrom(listenSocket, (char*)&buff, 1024, 0, (sockaddr *)&remoteAddr, &nAddrLen);
        if(ret > 0)
        {
            fileAck ack;
            ack.number = buff.number;
            //qDebug("收到包：%d",ack.number);
            if(recievedNumber+1 != ack.number)//不需要这个块
            {
                qDebug("舍弃包: %d | 总包：%d",ack.number,filePackNumber_);
                ack.number = recievedNumber;
                sendto(listenSocket,reinterpret_cast<char*>(&ack),sizeof(fileAck),0,(sockaddr *)&remoteAddr, nAddrLen);
            }
            else//接受块
            {
                recievedNumber = ack.number;
                qDebug("接受包： %d | 总包：%d",recievedNumber,filePackNumber_);
                sendto(listenSocket,reinterpret_cast<char*>(&ack),sizeof(fileAck),0,(sockaddr *)&remoteAddr, nAddrLen);
                //qDebug("接受确认： ACK=%d",ack.number);
                //qDebug("Write data");
                memcpy(buffer_s,buff.body,buff.datalen);
                buffer_s += buff.datalen;//指针后移
                double progress = (double)recievedNumber/filePackNumber_*100;
                emit progressTo(progress);
                if(recievedNumber == filePackNumber_)
                {
                    closesocket(listenSocket);
                    break;//接收完毕
                }
            }

        }
    }
    double timem = time1.elapsed();
    double speed = filesize/(timem/1000)/1024;
    qDebug("speed= %f KB/s",speed);
    QFile file(filename);
    file.open(QFile::WriteOnly);
    file.write(buffer,filesize);
    file.close();
    delete []buffer;
    buffer = nullptr;
    qDebug("接收完毕，接收线程退出");
    return;

}

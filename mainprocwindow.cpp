#include "mainprocwindow.h"
#include "ui_mainprocwindow.h"
#include "stdafx.h"

mainProcWindow::mainProcWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mainProcWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowIcon(QIcon(":/img/background/res/simplechat.ico"));
    setWindowTitle("SimpleChat");
    setAttribute(Qt::WA_TranslucentBackground);
    GetPrivateProfileString(L"SERVER", L"IP", L"127.0.0.1", serverIP, 16, configPath);
    serverPort = GetPrivateProfileInt(L"SERVER", L"PORT", 12000, configPath);//有点问题
    state = true;
    ui->starButton->setStyleSheet("QPushButton{border:0px;font: 9pt '微软雅黑';border-top-left-radius:5px;border-bottom-left-radius:5px;background-color: rgba(21, 173, 235,10%);}\
                                   QPushButton:hover{background-color: rgba(21, 173, 235,30%);}\
                                   QPushButton:pressed{background-color: rgba(21, 173, 235,50%);}");
    QObject::connect(this,SIGNAL(stateNow(bool)),this,SLOT(enterOffline(bool)));
}

mainProcWindow::~mainProcWindow()
{
    delete updateTimer;
    delete listModel;
    for(chatWindow* i : chatWs){
        i->close();
        delete i;
    }
    delete ui;
}

void mainProcWindow::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        postion = event->pos();
        if(postion.y() <= 40 && postion.x() <= 230)
        {
            headpressed = true;
        }
    }
}

void mainProcWindow::mouseMoveEvent(QMouseEvent* event)
{
    if(headpressed)
    {
        move(event->pos()-postion+pos());
    }
}

void mainProcWindow::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    headpressed = false;
}

void mainProcWindow::on_closeButton_clicked()
{
    if(state)emit stateNow(false);
    this->close();
}

void mainProcWindow::on_minButton_clicked()
{
    this->showMinimized();
}

void mainProcWindow::updateinfo()
{
    //update online state
    if(state)
    {
        stateMessage stateMsg, recvMsg;
        stateMsg.msgType = STATE;
        stateMsg.accountID = account;
        stateMsg.session = session;
        stateMsg.keepAlive = true;
        myTcpSocket logoutSocket;
        logoutSocket.config(serverIP, serverPort);
        logoutSocket.connectToHost();
        logoutSocket.sendMsg(reinterpret_cast<char*>(&stateMsg));
        logoutSocket.recvMsg(reinterpret_cast<char*>(&recvMsg));
        if(recvMsg.session == 0)//离线状态
        {
            emit stateNow(false);
        }
        else state = true;
        logoutSocket.disconnect();
    }
    else emit stateNow(false);
}

void mainProcWindow::enterOffline(bool control)
{
    if(!control)
    {
        ui->stateButton->setStyleSheet("border:0px;color: rgb(130, 135, 131);;");
        state = false;
    }
    else
    {
        state = true;
    }
}


void mainProcWindow::initConfig(unsigned accounts, unsigned sessions, unsigned port)
{
    unsigned long tip;
    unsigned tport;
    qDebug("MainProcWindowInit:account=%d,session=%d,localPort=%d",accounts,sessions,port);
    updateTimer = new QTimer(this);//update
    QObject::connect(updateTimer,SIGNAL(timeout()),this,SLOT(updateinfo()));
    updateTimer->start(10000);
    account = accounts;
    session = sessions;
    localPort = port;
    configServer();
    QObject::connect(this,SIGNAL(clientNow(int)),this,SLOT(acceptOne(int)));
    std::thread tAccept(&mainProcWindow::acceptThread,this);
    tAccept.detach();
    requestModule.config(accounts,sessions);
    requestModule.getUserinfo(account,username,tip,tport);
    ui->usernamelabel->setText(QString(username));
    listModel = new QStringListModel;
    listModel->setStringList(histroyList);
    ui->listView->setModel(listModel);
    QFile qss(":/img/background/listView.qss");
    qss.open(QFile::ReadOnly);
    ui->listView->setStyleSheet(qss.readAll());
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    qss.close();
}

void mainProcWindow::on_stateButton_clicked()
{
    if(state)
    {
        ui->stateButton->setStyleSheet("border:0px;color: rgb(130, 135, 131);");
        state = false;
        updateTimer->stop();
        delete updateTimer;
        emit stateNow(false);
    }
    else
    {
        ui->stateButton->setStyleSheet("border:0px;color: rgb(0, 255, 0);");
        state = true;
        emit stateNow(true);
    }
}

void mainProcWindow::on_starButton_clicked()
{
    char username_t[8];
    unsigned long ip_t;
    unsigned port_t;
    if(ui->lineEdit->text().toInt() == account)
    {
        ui->lineEdit->setText("不能与自己聊天");
        return;
    }
    switch(requestModule.getUserinfo(ui->lineEdit->text().toInt(),username_t,ip_t,port_t))
    {
    case 0:{
        bool result = true;
        for(chatWindow* i:chatWs)//检查是否已有对应窗口
        {
            if(i->account == ui->lineEdit->text().toInt()){
                qDebug("Log: Find WINDOW");
                i->setWindow(QString(username_t),ip_t,port_t,ui->lineEdit->text().toInt(),account,session);
                ui->lineEdit->setText("");
                i->show();
                result = false;
                break;
            }
        }
        if(result)
        {
            QString info("  用户名: ");
            info.append(username_t);
            info.append("\n  账号: ");
            info.append(ui->lineEdit->text());
            QStringList temp(info.toStdString().data());
            histroyList = temp.operator +(histroyList);
            listModel->setStringList(histroyList);
            accountList.insert(accountList.begin(),ui->lineEdit->text().toInt());
            chatWindow* w = new chatWindow;
            QObject::connect(w,SIGNAL(refreshChatW(uint)),this,SLOT(torefreshChatW(uint)));
            chatWs.push_back(w);
            w->setWindow(QString(username_t),ip_t,port_t,ui->lineEdit->text().toInt(),account,session);
            ui->lineEdit->setText("");
            w->show();
            //w->exec();
        }
        break;
    }
    case ERROR_OFFLINE:{
        bool result = true;
        for(chatWindow* i:chatWs)//检查是否已有对应窗口
        {
            if(i->account == ui->lineEdit->text().toInt()){
                qDebug("Log: Find WINDOW");
                i->setWindow(QString(username_t),toIPint(serverIP),serverPort,ui->lineEdit->text().toInt(),account,session);
                ui->lineEdit->setText("");
                i->show();
                result = false;
                break;
            }
        }
        if(result)
        {
            QString info("  用户名: ");
            info.append(username_t);
            info.append("\n  账号: ");
            info.append(ui->lineEdit->text());
            QStringList temp(info.toStdString().data());
            histroyList = temp.operator +(histroyList);
            listModel->setStringList(histroyList);
            accountList.insert(accountList.begin(),ui->lineEdit->text().toInt());
            chatWindow* w = new chatWindow;
            QObject::connect(w,SIGNAL(refreshChatW(uint)),this,SLOT(torefreshChatW(uint)));
            chatWs.push_back(w);
            w->setWindow(QString(username_t),toIPint(serverIP),serverPort,ui->lineEdit->text().toInt(),account,session);
            ui->lineEdit->setText("");
            w->show();
            //w->exec();
        }
        break;
    }
    default:
    {
        ui->lineEdit->setText("查询该账号信息失败");
        return;
    }
    }


}

void mainProcWindow::torefreshChatW(unsigned account)
{
    for(chatWindow* i:chatWs)
    {
        if(i->account == account)
        {
            char username_t[8];
            unsigned long ip_t;
            unsigned port_t;
            switch(requestModule.getUserinfo(account,username_t,ip_t,port_t))
            {
            case 0:i->setWindow(QString(username_t),ip_t,port_t,account,this->account,session);break;
            case ERROR_OFFLINE:i->setWindow(QString(username_t),toIPint(serverIP),serverPort,account,this->account,session);
            }
            break;
        }
    }
}

bool mainProcWindow::configServer()//config socket setting, ready to connect
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
    addr.sin_port = htons(localPort);
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
    return true;
}

void mainProcWindow::acceptThread()
{
    while(true)
    {
        sockaddr_in clientAddr;
        int length = sizeof(clientAddr);
        SOCKET clientSocket = ::accept(mainSocket, (sockaddr*)&clientAddr,&length);
        emit clientNow((int)clientSocket);
    }

}

unsigned long mainProcWindow::toIPint(WCHAR *ip)
{
    char temp[16];
    WideCharToMultiByte(CP_ACP,0,ip,-1,temp,16,NULL,NULL);
    return inet_addr(temp);
}

int mainProcWindow::addHistroy(QString qsusername, unsigned accountid)
{

}

void mainProcWindow::acceptOne(int clientSockets)//connect to the host
{
    SOCKET clientSocket = (SOCKET)clientSockets;
    if(clientSocket == INVALID_SOCKET)
    {
        qDebug("Accept failed");
    }
    else
    {
        qDebug("Log: Accept Client");
        basicMessage recvMsg;
        errorCode = recv(clientSocket,reinterpret_cast<char*>(&recvMsg),128,0);
        qDebug("Log: Recieve message");
        if (errorCode == SOCKET_ERROR)
        {
            qDebug("Recv failed");
        }
        else
        {
            if(recvMsg.msgType == FILEREQ)
            {
                //文件传输请求
                basicMessage resMsg;
                resMsg.msgType = 1;
                send(clientSocket,reinterpret_cast<char*>(&resMsg),sizeof(basicMessage),0);//发送一个包表示收到
                char username_t[8];
                unsigned long ip_t;
                unsigned port_t;
                bool result = true;
                qDebug("Log: 文件传输请求 from %d",recvMsg.accountID);
                for(chatWindow* i:chatWs)//检查是否已有对应窗口
                {
                    if(i->account == recvMsg.accountID){
                        qDebug("Log: Find WINDOW");
                        requestModule.getUserinfo(recvMsg.accountID,username_t,ip_t,port_t);
                        i->setWindow(QString(username_t),ip_t,port_t,recvMsg.accountID,account,session);
                        i->setFileInfo(QString(recvMsg.body),recvMsg.session,recvMsg.targetID,localPort);
                        i->show();
                        result = false;
                        break;
                    }
                }
                if(result)
                {
                    qDebug("Log: Create WINDOW");
                    chatWindow* w = new chatWindow;
                    QObject::connect(w,SIGNAL(refreshChatW(uint)),this,SLOT(torefreshChatW(uint)));
                    chatWs.push_back(w);
                    requestModule.getUserinfo(recvMsg.accountID,username_t,ip_t,port_t);
                    w->setWindow(QString(username_t),ip_t,port_t,recvMsg.accountID,account,session);
                    w->setFileInfo(QString(recvMsg.body),recvMsg.session,recvMsg.targetID,localPort);
                    w->show();
                    //w->exec();

                    QString info("  用户名: ");
                    info.append(username_t);
                    info.append("\n  账号: ");
                    info.append(QString::number(recvMsg.accountID));
                    QStringList temp(info.toStdString().data());
                    histroyList = temp.operator +(histroyList);
                    listModel->setStringList(histroyList);
                    accountList.insert(accountList.begin(),recvMsg.accountID);
                }
            }
            else
            {
                //普通消息信息
                char username_t[8];
                unsigned long ip_t;
                unsigned port_t;
                bool result = true;
                qDebug("Log: Check exist window");
                for(chatWindow* i:chatWs)//检查是否已有对应窗口
                {
                    if(i->account == recvMsg.accountID){
                        qDebug("Log: Find WINDOW");
                        switch(requestModule.getUserinfo(recvMsg.accountID,username_t,ip_t,port_t))
                        {
                        case 0:{
                            i->setWindow(QString(username_t),ip_t,port_t,recvMsg.accountID,account,session);
                            i->addMessage(QString(recvMsg.body));
                            i->show();
                            break;
                        }
                        case ERROR_OFFLINE:{
                            i->setWindow(QString(username_t),toIPint(serverIP),serverPort,recvMsg.accountID,account,session);
                            i->addMessage(QString(recvMsg.body));
                            i->show();
                            break;

                        }
                        }
                        result = false;
                        break;
                    }
                }
                if(result)
                {
                    qDebug("Log: Create WINDOW");
                    chatWindow* w = new chatWindow;
                    QObject::connect(w,SIGNAL(refreshChatW(uint)),this,SLOT(torefreshChatW(uint)));
                    chatWs.push_back(w);
                    switch(requestModule.getUserinfo(recvMsg.accountID,username_t,ip_t,port_t))
                    {
                    case 0:{
                        w->setWindow(QString(username_t),ip_t,port_t,recvMsg.accountID,account,session);
                        w->addMessage(QString(recvMsg.body));
                        w->show();
                        //w->exec();
                        break;
                    }
                    case ERROR_OFFLINE:{
                        w->setWindow(QString(username_t),toIPint(serverIP),serverPort,recvMsg.accountID,account,session);
                        w->addMessage(QString(recvMsg.body));
                        w->show();
                        //w->exec();
                        break;

                    }
                    }
                    QString info("  用户名: ");
                    info.append(username_t);
                    info.append("\n  账号: ");
                    info.append(QString::number(recvMsg.accountID));
                    QStringList temp(info.toStdString().data());
                    histroyList = temp.operator +(histroyList);
                    listModel->setStringList(histroyList);
                    accountList.insert(accountList.begin(),recvMsg.accountID);
                }
            }

        }
    }
}

void mainProcWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    for(chatWindow* i:chatWs)
    {
        if(accountList[index.row()] == i->account)
        {
            i->show();
            break;
        }
    }

}

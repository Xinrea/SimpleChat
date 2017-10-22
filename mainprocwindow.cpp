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
    delete ui;
    delete updateTimer;
    for(auto i : chatWs)delete i;
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
    qDebug("updateinfo()");
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
    requestModule.config(accounts,sessions);
    requestModule.getUserinfo(account,username,tip,tport);
    ui->usernamelabel->setText(QString(username));
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
    chatWindow* w = new chatWindow;
    chatWs.push_back(w);
    char username_t[8];
    unsigned long ip_t;
    unsigned port_t;
    if(!requestModule.getUserinfo(ui->lineEdit->text().toInt(),username_t,ip_t,port_t))
    {
        w->setWindow(QString(username_t),ip_t,port_t);
        w->show();
        w->exec();
    }
    else
    {
        ui->lineEdit->setText("查询该账号信息失败");
    }

}

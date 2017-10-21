#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "registerwindow.h"
#include "findpwdwindow.h"
#include "mlogin.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowIcon(QIcon(":/img/background/res/simplechat.ico"));
    setAttribute(Qt::WA_TranslucentBackground);
    ui->registerButton->setStyleSheet("QPushButton{background-image: url();\
                                      font: 9pt '微软雅黑';\
                                      border:0px;\
                                      color: rgb(255, 255, 255);}\
                                      QPushButton:hover{color: rgb(170, 255, 255);}");
    ui->findpwdButton->setStyleSheet("QPushButton{background-image: url();\
                                      font: 9pt '微软雅黑';\
                                      border:0px;\
                                      color: rgb(255, 255, 255);}\
                                      QPushButton:hover{color: rgb(170, 255, 255);}");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_closeButton_clicked()
{
    this->close();
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        postion = event->pos();
        if(postion.y() <= 60 && postion.x() <= 400)
        {
            headpressed = true;
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    if(headpressed)
    {
        move(event->pos()-postion+pos());
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    headpressed = false;
}

void MainWindow::on_miniButton_clicked()
{
    this->showMinimized();
}

void MainWindow::on_loginButton_clicked()
{
    mlogin loginModule;
    unsigned account = ui->accountEdit->text().toInt();
    char password[PASSLEN];
    ui->label->setText("");
    if(account >= 65535){
        ui->label->setText("登录错误:  账号输入错误");
    }
    else
    {
        strcpy(password,ui->passwordEdit->text().toStdString().data());
        ui->loginButton->setDisabled(true);//进入等待状态
        ui->accountEdit->setDisabled(true);
        ui->passwordEdit->setDisabled(true);
        ui->findpwdButton->setDisabled(true);
        ui->registerButton->setDisabled(true);
        switch(loginModule.login(account,password))
        {
        case CONNECTERROR: {
            ui->label->setText("登录错误:  连接服务器错误");
            break;
            }
        case SENDERROR: {
            ui->label->setText("登录错误:  发送消息错误");
            break;
            }
        case RECVERROR: {
            ui->label->setText("登录错误:  帐号密码错误");
            break;
            }
        case 0: session = loginModule.getSession();break;
        }
        ui->loginButton->setDisabled(false);//恢复状态
        ui->accountEdit->setDisabled(false);
        ui->passwordEdit->setDisabled(false);
        ui->findpwdButton->setDisabled(false);
        ui->registerButton->setDisabled(false);
    }

}


void MainWindow::on_accountEdit_textChanged(const QString &arg1)
{
    ui->label->setText("");
}

void MainWindow::on_registerButton_clicked()
{
    registerWindow registerW;
    registerW.show();
    registerW.exec();
}

void MainWindow::on_findpwdButton_clicked()
{
    findpwdWindow findpwdW;
    findpwdW.show();
    findpwdW.exec();
}

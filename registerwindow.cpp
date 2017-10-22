#include "registerwindow.h"
#include "ui_registerwindow.h"
#include "mregister.h"

registerWindow::registerWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::registerWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowModality(Qt::ApplicationModal);
    setWindowIcon(QIcon(":/img/background/res/simplechat.ico"));
    setWindowTitle("Register - SimpleChat");
    setAttribute(Qt::WA_TranslucentBackground);
}

registerWindow::~registerWindow()
{
    delete ui;
}

void registerWindow::on_pushButton_clicked()
{
    this->close();
}

void registerWindow::mousePressEvent(QMouseEvent* event)
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

void registerWindow::mouseMoveEvent(QMouseEvent* event)
{
    if(headpressed)
    {
        move(event->pos()-postion+pos());
    }
}

void registerWindow::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    headpressed = false;
}

void registerWindow::on_pushButton_2_clicked()
{
    mregister registerModule;
    char username[8];
    unsigned accountID;
    ui->infoLabel->setText("");
    if(ui->usernameEdit->text().length() == 0)
    {
        ui->infoLabel->setText("请输入用户名");
        return;
    }
    else if(ui->passwordEdit->text().length() == 0)
    {
        ui->infoLabel->setText("请输入密码");
        return;
    }
    else if(ui->comfirmEdit->text().length() == 0)
    {
        ui->infoLabel->setText("请输入验证信息");
        return;
    }

    switch(registerModule.registerOnServer(ui->usernameEdit->text().toStdString().data(),ui->passwordEdit->text().toStdString().data(),ui->comfirmEdit->text().toStdString().data(),accountID))
    {
    case ERROR_SOCKETCONNECT: {
        ui->infoLabel->setText("注册错误:  连接服务器错误");
        break;
        }
    case ERROR_SOCKETSEND: {
        ui->infoLabel->setText("注册错误:  发送消息错误");
        break;
        }
    case ERROR_SOCKETRECV: {
        ui->infoLabel->setText("注册错误:  接受消息错误");
        break;
        }
    case 0: {
        QString temp;
        temp.sprintf("注册成功，所得账号为：%d",accountID);
        ui->infoLabel->setText(temp);
        break;
        }
    }

}

void registerWindow::on_usernameEdit_textChanged(const QString &arg1)
{
    ui->infoLabel->setText("");
}

void registerWindow::on_passwordEdit_textChanged(const QString &arg1)
{
    ui->infoLabel->setText("");
}

void registerWindow::on_comfirmEdit_textChanged(const QString &arg1)
{
    ui->infoLabel->setText("");
}

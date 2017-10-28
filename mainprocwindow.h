#ifndef MAINPROCWINDOW_H
#define MAINPROCWINDOW_H

#include <QDialog>
#include <QMouseEvent>
#include <QTimer>
#include <QStringListModel>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include "msgstruct.h"
#include "mytcpsocket.h"
#include "chatwindow.h"
#include "mrequest.h"

namespace Ui {
class mainProcWindow;
}

class mainProcWindow : public QDialog
{
    Q_OBJECT

public:
    explicit mainProcWindow(QWidget *parent = 0);
    void initConfig(unsigned accounts,unsigned sessions,unsigned port);
    ~mainProcWindow();
private:
    bool configServer();
    void acceptThread();
    unsigned long toIPint(WCHAR* ip);
    int addHistroy(QString qsusername,unsigned accountid);
signals:
    void stateNow(bool state);
    void clientNow(int clientSocket);
    void fileTo(char* buff);
private slots:
    void on_closeButton_clicked();

    void on_minButton_clicked();

    void updateinfo();

    void enterOffline(bool control);

    void on_stateButton_clicked();

    void on_starButton_clicked();

    void torefreshChatW(unsigned account);

    void acceptOne(int clientSockets);
    void on_listView_doubleClicked(const QModelIndex &index);

private:
    WSADATA wsaData;
    sockaddr_in addr;
    SOCKET mainSocket;
    int errorCode;

    Ui::mainProcWindow *ui;
    bool headpressed;
    bool state;
    QPoint postion;
    char username[8];
    unsigned account;
    unsigned session;
    unsigned localPort;
    mrequest requestModule;
    WCHAR serverIP[16];
    int serverPort;
    WCHAR configPath[25] = L".//config.ini";
    QTimer *updateTimer,*listenTimer;
    std::vector<chatWindow*> chatWs;
    QStringListModel* listModel;
    QStringList histroyList;
    std::vector<int> accountList;
protected:
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
};

#endif // MAINPROCWINDOW_H

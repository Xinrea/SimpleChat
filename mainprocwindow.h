#ifndef MAINPROCWINDOW_H
#define MAINPROCWINDOW_H

#include <QDialog>
#include <QMouseEvent>
#include <QTimer>
#include <vector>
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
signals:
    void stateNow(bool state);
private slots:
    void on_closeButton_clicked();

    void on_minButton_clicked();

    void updateinfo();

    void enterOffline(bool control);

    void on_stateButton_clicked();

    void on_starButton_clicked();

private:
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
    QTimer *updateTimer;
    std::vector<chatWindow*> chatWs;
protected:
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
};

#endif // MAINPROCWINDOW_H

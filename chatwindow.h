#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QDialog>
#include <QMouseEvent>
namespace Ui {
class chatWindow;
}

class chatWindow : public QDialog
{
    Q_OBJECT

public:
    explicit chatWindow(QWidget *parent = 0);
    void setWindow(QString name,unsigned long ip,unsigned port);
    ~chatWindow();

private:
    Ui::chatWindow *ui;
    bool headpressed;
    void intIPtoStr(unsigned long ip,char* buf);
    QPoint postion;
protected:
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
private slots:
    void on_pushButton_clicked();
};

#endif // CHATWINDOW_H

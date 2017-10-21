#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_closeButton_clicked();

    void on_miniButton_clicked();

    void on_loginButton_clicked();

    void on_accountEdit_textChanged(const QString &arg1);

    void on_registerButton_clicked();

    void on_findpwdButton_clicked();

private:
    Ui::MainWindow *ui;
    bool headpressed;
    QPoint postion;
    int session;
protected:
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
};

#endif // MAINWINDOW_H

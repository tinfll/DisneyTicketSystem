#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

// 前向声明，避免头文件包含
class QueryDialog;
class BuyTicketDialog;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_queryButton_clicked();      // 查询票价按钮
    void on_buyTicketButton_clicked();  // 购买门票按钮
    void on_exitButton_clicked();       // 退出系统按钮

private:
    Ui::MainWindow *ui;
    QueryDialog *queryDialog;           // 查询对话框
    BuyTicketDialog *buyTicketDialog;   // 购票对话框
};

#endif // MAINWINDOW_H

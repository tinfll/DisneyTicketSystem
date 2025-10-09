#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "QueryDialog.h"
#include "BuyTicketDialog.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , queryDialog(nullptr)
    , buyTicketDialog(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("游乐园票务系统");

    // 连接按钮信号和槽函数
    connect(ui->queryButton, &QPushButton::clicked, this, &MainWindow::on_queryButton_clicked);
    connect(ui->buyTicketButton, &QPushButton::clicked, this, &MainWindow::on_buyTicketButton_clicked);
    connect(ui->exitButton, &QPushButton::clicked, this, &MainWindow::on_exitButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete queryDialog;
    delete buyTicketDialog;
}

void MainWindow::on_queryButton_clicked()
{
    if (!queryDialog) {
        queryDialog = new QueryDialog(this);
    }
    queryDialog->show();
    queryDialog->raise();
    queryDialog->activateWindow();
}

void MainWindow::on_buyTicketButton_clicked()
{
    if (!buyTicketDialog) {
        buyTicketDialog = new BuyTicketDialog(this);
    }
    buyTicketDialog->show();
    buyTicketDialog->raise();
    buyTicketDialog->activateWindow();
}

void MainWindow::on_exitButton_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认退出", "确定要退出系统吗？",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        QApplication::quit();
    }
}

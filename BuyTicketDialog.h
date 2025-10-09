#ifndef BUYTICKETDIALOG_H
#define BUYTICKETDIALOG_H

#include <QDialog>

namespace Ui {
class BuyTicketDialog;
}

class BuyTicketDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BuyTicketDialog(QWidget *parent = nullptr);
    ~BuyTicketDialog();

private slots:
    void on_calculateButton_clicked();  // 计算票价按钮
    void on_buyButton_clicked();        // 确认购买按钮
    void on_clearButton_clicked();      // 清空按钮
    void on_backButton_clicked();       // 返回按钮

private:
    Ui::BuyTicketDialog *ui;

    // 票价数据
    double childPrices[4] = {375, 425, 599, 699};
    double adultPrices[4] = {475, 525, 699, 799};
    double seniorPrices[4] = {350, 400, 550, 650};
    double disablePrices[4] = {338, 383, 539, 629};

    // 辅助函数
    int calculateAge(const QDate& birthDate, const QDate& visitDate);
    QString getTicketType(int age, bool hasDisableCert);
    int getDateType(const QString& date);
    double calculateTotalPrice(const QString& ticketType, int dateType, int ticketDays, int quantity);
};

#endif // BUYTICKETDIALOG_H

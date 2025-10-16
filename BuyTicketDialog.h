#ifndef BUYTICKETDIALOG_H
#define BUYTICKETDIALOG_H

#include <QDialog>
#include <QList>

// 门票项数据结构
struct TicketItem {
    QString birthDate;
    int age;
    QString visitDate;
    QString ticketType;
    int ticketDays;
    int quantity;
    double price;
    double totalPrice;
};

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
    void on_calculateButton_clicked();        // 计算单张票价并添加到购物车
    void on_buyButton_clicked();              // 确认购买所有门票
    void on_clearButton_clicked();            // 清空购物车
    void on_backButton_clicked();             // 返回按钮
    void on_removeButton_clicked();           // 移除选中项目

private:
    Ui::BuyTicketDialog *ui;
    QList<TicketItem> cartItems;              // 购物车列表

    // 票价数据
    double childPrices[4] = {356, 404, 494, 599};
    double adultPrices[4] = {475, 539, 659, 799};
    double seniorPrices[4] = {356, 404, 494, 599};
    double disablePrices[4] = {428, 485, 593, 719};

    // 辅助函数
    int calculateAge(const QDate& birthDate, const QDate& visitDate);
    QString getTicketType(int age, bool hasDisableCert);
    int getDateType(const QString& date);
    double calculateTotalPrice(const QString& ticketType, int dateType, int ticketDays, int quantity);
    void updateCartDisplay();                 // 更新购物车显示
    double calculateGrandTotal();             // 计算购物车总价

    bool validateDate(int year, int month, int day);
};

#endif // BUYTICKETDIALOG_H

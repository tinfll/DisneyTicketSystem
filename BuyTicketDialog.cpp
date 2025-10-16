#include "BuyTicketDialog.h"
#include "ui_BuyTicketDialog.h"
#include <QDate>
#include <QMessageBox>

BuyTicketDialog::BuyTicketDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BuyTicketDialog)
{
    ui->setupUi(this);
    setWindowTitle("购买门票");

    // 设置日期选择器
    ui->birthDateEdit->setDate(QDate::currentDate().addYears(-20));
    ui->birthDateEdit->setCalendarPopup(true);  // 添加日历弹出

    ui->visitDateEdit->setDate(QDate::currentDate());
    ui->visitDateEdit->setMinimumDate(QDate::currentDate());
    ui->visitDateEdit->setMaximumDate(QDate::currentDate().addYears(1));  // 允许选择未来一年
    ui->visitDateEdit->setCalendarPopup(true);  // 添加日历弹出

    // 设置数量选择器
    ui->quantitySpinBox->setRange(1, 10);
    ui->quantitySpinBox->setValue(1);
    ui->quantitySpinBox->setWrapping(true);  // 允许循环


    // 连接按钮信号
    /*connect(ui->calculateButton, &QPushButton::clicked, this, &BuyTicketDialog::on_calculateButton_clicked);
    connect(ui->buyButton, &QPushButton::clicked, this, &BuyTicketDialog::on_buyButton_clicked);
    connect(ui->clearButton, &QPushButton::clicked, this, &BuyTicketDialog::on_clearButton_clicked);
    connect(ui->backButton, &QPushButton::clicked, this, &BuyTicketDialog::on_backButton_clicked);*/

    // 新增：移除按钮
    //connect(ui->removeButton, &QPushButton::clicked, this, &BuyTicketDialog::on_removeButton_clicked);

    // 初始化购物车显示
    ui->cartText->setPlainText("购物车为空");
}

BuyTicketDialog::~BuyTicketDialog()
{
    delete ui;
}
// 引入组员的日期验证函数
bool BuyTicketDialog::validateDate(int year, int month, int day)
{

    switch (month) {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
        return (day <= 31 && day > 0);
    case 4: case 6: case 9: case 11:
        return (day <= 30 && day > 0);
    case 2:
        if (year % 4 == 0) {
            return (day <= 29 && day > 0);
        } else {
            return (day <= 28 && day > 0);
        }
    default:
        return false;
    }
}



void BuyTicketDialog::on_calculateButton_clicked()
{
    // 获取输入数据
    QDate birthDate = ui->birthDateEdit->date();
    QDate visitDate = ui->visitDateEdit->date();
    int ticketDays = ui->ticketTypeComboBox->currentIndex() + 1;
    int quantity = ui->quantitySpinBox->value();
    bool hasDisableCert = ui->disableCheckBox->isChecked();

    // 计算年龄和票种
    int age = calculateAge(birthDate, visitDate);
    QString ticketType = getTicketType(age, hasDisableCert);
    int dateType = getDateType(visitDate.toString("yyyy-MM-dd"));

    if (ticketType == "婴儿") {
        QMessageBox::information(this, "提示", "3岁以下婴儿免费入园，无需添加到购物车！");
        return;
    }

    if (!validateDate(birthDate.year(), birthDate.month(), birthDate.day()) ||
        !validateDate(visitDate.year(), visitDate.month(), visitDate.day())) {
        QMessageBox::warning(this, "日期错误", "输入的日期不合法！");
        return;
    }
    // 计算单价和总价
    double price = calculateTotalPrice(ticketType, dateType, ticketDays, 1); // 计算单价
    double totalPrice = price * quantity;

    // 创建门票项并添加到购物车
    TicketItem item;
    item.birthDate = birthDate.toString("yyyy年MM月dd日");
    item.age = age;
    item.visitDate = visitDate.toString("yyyy年MM月dd日");
    item.ticketType = ticketType;
    item.ticketDays = ticketDays;
    item.quantity = quantity;
    item.price = price / quantity; // 单张门票单价
    item.totalPrice = totalPrice;

    cartItems.append(item);

    // 更新购物车显示
    updateCartDisplay();

    // 显示添加成功提示
    QMessageBox::information(this, "成功", "已添加到购物车！");
}

void BuyTicketDialog::on_buyButton_clicked()
{
    if (cartItems.isEmpty()) {
        QMessageBox::warning(this, "提示", "购物车为空，请先添加门票！");
        return;
    }

    double grandTotal = calculateGrandTotal();

    QString message = QString("确认购买 %1 张门票？\n总计金额: %2 元")
                          .arg(cartItems.size())
                          .arg(grandTotal);

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认购买", message,
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QMessageBox::information(this, "购票成功",
                                 QString("门票购买成功！\n共计 %1 张门票\n总金额: %2 元")
                                     .arg(cartItems.size())
                                     .arg(grandTotal));
        on_clearButton_clicked();
    }
}

void BuyTicketDialog::on_clearButton_clicked()
{
    cartItems.clear();
    updateCartDisplay();
    ui->birthDateEdit->setDate(QDate::currentDate().addYears(-20));
    ui->visitDateEdit->setDate(QDate::currentDate());
    ui->ticketTypeComboBox->setCurrentIndex(0);
    ui->quantitySpinBox->setValue(1);
    ui->disableCheckBox->setChecked(false);
}

void BuyTicketDialog::on_removeButton_clicked()
{
    // 这里可以实现在购物车中移除特定项目的逻辑
    // 暂时简单实现为清空购物车
    if (!cartItems.isEmpty()) {
        cartItems.removeLast();
        updateCartDisplay();
    }
}

void BuyTicketDialog::on_backButton_clicked()
{
    this->hide();
}

void BuyTicketDialog::updateCartDisplay()
{
    if (cartItems.isEmpty()) {
        ui->cartText->setPlainText("购物车为空");
        return;
    }

    QString cartContent = "=== 购物车内容 ===\n";
    double grandTotal = 0;

    for (int i = 0; i < cartItems.size(); ++i) {
        const TicketItem& item = cartItems[i];
        cartContent += QString("%1. %2票 %3日票 ×%4 - %5元\n")
                           .arg(i + 1)
                           .arg(item.ticketType)
                           .arg(item.ticketDays)
                           .arg(item.quantity)
                           .arg(item.totalPrice);
        grandTotal += item.totalPrice;
    }

    cartContent += "===================\n";
    cartContent += QString("总计: %1 元").arg(grandTotal);

    ui->cartText->setPlainText(cartContent);
}

double BuyTicketDialog::calculateGrandTotal()
{
    double total = 0;
    for (const TicketItem& item : cartItems) {
        total += item.totalPrice;
    }
    return total;
}

// 以下辅助函数保持不变
int BuyTicketDialog::calculateAge(const QDate& birthDate, const QDate& visitDate)
{
    int age = visitDate.year() - birthDate.year();
    if (visitDate.month() < birthDate.month() ||
        (visitDate.month() == birthDate.month() && visitDate.day() < birthDate.day())) {
        age--;
    }
    return age;
}

QString BuyTicketDialog::getTicketType(int age, bool hasDisableCert)
{
    if (age < 3) return "婴儿";
    if (hasDisableCert) return "残障";
    if (age <= 11) return "儿童";
    if (age >= 60) return "长者";
    return "标准";
}

int BuyTicketDialog::getDateType(const QString& date)
{
    QDate qDate = QDate::fromString(date, "yyyy-MM-dd");
    int dayOfWeek = qDate.dayOfWeek();
    int month = qDate.month();
    int day = qDate.day();

    if ((month == 1 && day >= 1 && day <= 3) ||
        (month == 5 && day >= 1 && day <= 3) ||
        (month == 10 && day >= 1 && day <= 7)) {
        return 3;
    }

    if (dayOfWeek == 6 || dayOfWeek == 7) return 2;
    if (dayOfWeek == 5) return 1;
    return 0;
}

double BuyTicketDialog::calculateTotalPrice(const QString& ticketType, int dateType, int ticketDays, int quantity)
{
    double price = 0;

    if (ticketType == "儿童") price = childPrices[dateType];
    else if (ticketType == "标准") price = adultPrices[dateType];
    else if (ticketType == "长者") price = seniorPrices[dateType];
    else if (ticketType == "残障") price = disablePrices[dateType];

    double total = price * quantity;
    if (ticketDays == 2) total *= 1.8;

    return total;
}

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
    ui->birthDateEdit->setDate(QDate::currentDate().addYears(-20)); // 默认20岁
    ui->visitDateEdit->setDate(QDate::currentDate());
    ui->visitDateEdit->setMinimumDate(QDate::currentDate());

    // 设置数量选择器
    ui->quantitySpinBox->setRange(1, 10);
    ui->quantitySpinBox->setValue(1);

    // 连接按钮信号
    connect(ui->calculateButton, &QPushButton::clicked, this, &BuyTicketDialog::on_calculateButton_clicked);
    connect(ui->buyButton, &QPushButton::clicked, this, &BuyTicketDialog::on_buyButton_clicked);
    connect(ui->clearButton, &QPushButton::clicked, this, &BuyTicketDialog::on_clearButton_clicked);
    connect(ui->backButton, &QPushButton::clicked, this, &BuyTicketDialog::on_backButton_clicked);
}

BuyTicketDialog::~BuyTicketDialog()
{
    delete ui;
}

void BuyTicketDialog::on_calculateButton_clicked()
{
    // 获取输入数据
    QDate birthDate = ui->birthDateEdit->date();
    QDate visitDate = ui->visitDateEdit->date();
    int ticketDays = ui->ticketTypeComboBox->currentIndex() + 1; // 0->1日票, 1->2日票
    int quantity = ui->quantitySpinBox->value();
    bool hasDisableCert = ui->disableCheckBox->isChecked();

    // 计算年龄和票种
    int age = calculateAge(birthDate, visitDate);
    QString ticketType = getTicketType(age, hasDisableCert);
    int dateType = getDateType(visitDate.toString("yyyy-MM-dd"));

    if (ticketType == "婴儿") {
        ui->resultText->setPlainText("3岁以下婴儿免费入园！");
        return;
    }

    // 计算总价
    double totalPrice = calculateTotalPrice(ticketType, dateType, ticketDays, quantity);

    // 显示结果
    QString result = QString(
                         "=== 票价计算结果 ===\n"
                         "出生日期: %1\n"
                         "年龄: %2 岁\n"
                         "入园日期: %3\n"
                         "票种: %4票\n"
                         "天数: %5日票\n"
                         "数量: %6 张\n"
                         "总计金额: %7 元\n"
                         "==================="
                         ).arg(birthDate.toString("yyyy年MM月dd日"))
                         .arg(age)
                         .arg(visitDate.toString("yyyy年MM月dd日"))
                         .arg(ticketType)
                         .arg(ticketDays)
                         .arg(quantity)
                         .arg(totalPrice);

    ui->resultText->setPlainText(result);
}

void BuyTicketDialog::on_buyButton_clicked()
{
    if (ui->resultText->toPlainText().isEmpty()) {
        QMessageBox::warning(this, "提示", "请先计算票价！");
        return;
    }

    QMessageBox::information(this, "购票成功", "门票购买成功！感谢您的购买！");
    on_clearButton_clicked();
}

void BuyTicketDialog::on_clearButton_clicked()
{
    ui->birthDateEdit->setDate(QDate::currentDate().addYears(-20));
    ui->visitDateEdit->setDate(QDate::currentDate());
    ui->ticketTypeComboBox->setCurrentIndex(0);
    ui->quantitySpinBox->setValue(1);
    ui->disableCheckBox->setChecked(false);
    ui->resultText->clear();
}

void BuyTicketDialog::on_backButton_clicked()
{
    this->hide();
}

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
    // 复用QueryDialog中的逻辑
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

#include "QueryDialog.h"
#include "ui_QueryDialog.h"
#include <QDate>
#include <QMessageBox>

QueryDialog::QueryDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::QueryDialog)
{
    ui->setupUi(this);
    setWindowTitle("票价查询");

    // 设置日期选择器的默认值和范围
    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateEdit->setMinimumDate(QDate::currentDate());

    // 连接按钮信号
    connect(ui->queryButton, &QPushButton::clicked, this, &QueryDialog::on_queryButton_clicked);
    connect(ui->clearButton, &QPushButton::clicked, this, &QueryDialog::on_clearButton_clicked);
    connect(ui->backButton, &QPushButton::clicked, this, &QueryDialog::on_backButton_clicked);
}

QueryDialog::~QueryDialog()
{
    delete ui;
}

void QueryDialog::on_queryButton_clicked()
{
    QDate visitDate = ui->dateEdit->date();
    QString dateStr = visitDate.toString("yyyy-MM-dd");

    int dateType = getDateType(dateStr);
    QString dateTypeStr = getDateTypeString(dateType);

    // 显示查询结果
    QString result = QString(
                         "=== 票价查询结果 ===\n"
                         "入园日期: %1 (%2)\n\n"
                         "儿童票(3-11岁): %3 元\n"
                         "标准票(12-59岁): %4 元\n"
                         "长者票(60岁及以上): %5 元\n"
                         "残障票(持有效证明): %6 元\n"
                         "===================="
                         ).arg(dateStr).arg(dateTypeStr)
                         .arg(childPrices[dateType])
                         .arg(adultPrices[dateType])
                         .arg(seniorPrices[dateType])
                         .arg(disablePrices[dateType]);

    ui->resultText->setPlainText(result);
}

void QueryDialog::on_clearButton_clicked()
{
    ui->dateEdit->setDate(QDate::currentDate());
    ui->resultText->clear();
}

void QueryDialog::on_backButton_clicked()
{
    this->hide();
}

int QueryDialog::getDateType(const QString& date)
{
    QDate qDate = QDate::fromString(date, "yyyy-MM-dd");
    int dayOfWeek = qDate.dayOfWeek(); // 1=周一, 7=周日

    int month = qDate.month();
    int day = qDate.day();

    // 特别高峰日判断
    if ((month == 1 && day >= 1 && day <= 3) ||
        (month == 5 && day >= 1 && day <= 3) ||
        (month == 10 && day >= 1 && day <= 7)) {
        return 3;
    }

    // 高峰日判断 (周末)
    if (dayOfWeek == 6 || dayOfWeek == 7) {
        return 2;
    }

    // 特别常规日判断 (周五)
    if (dayOfWeek == 5) {
        return 1;
    }

    return 0; // 常规日
}

QString QueryDialog::getDateTypeString(int dateType)
{
    switch(dateType) {
    case 0: return "常规日";
    case 1: return "特别常规日";
    case 2: return "高峰日";
    case 3: return "特别高峰日";
    default: return "未知";
    }
}

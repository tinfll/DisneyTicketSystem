#ifndef QUERYDIALOG_H
#define QUERYDIALOG_H

#include <QDialog>

namespace Ui {
class QueryDialog;
}

class QueryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QueryDialog(QWidget *parent = nullptr);
    ~QueryDialog();

private slots:
    void on_queryButton_clicked();    // 查询按钮
    void on_clearButton_clicked();    // 清空按钮
    void on_backButton_clicked();     // 返回按钮

private:
    Ui::QueryDialog *ui;

    // 票价数据
    double childPrices[4] = {356, 404, 494, 599};
    double adultPrices[4] = {475, 539, 659, 799};
    double seniorPrices[4] = {356, 404, 494, 599};
    double disablePrices[4] = {428, 485, 593, 719};

    // 辅助函数
    int getDateType(const QString& date);
    QString getDateTypeString(int dateType);
};

#endif // QUERYDIALOG_H

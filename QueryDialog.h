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
    double childPrices[4] = {375, 425, 599, 699};
    double adultPrices[4] = {475, 525, 699, 799};
    double seniorPrices[4] = {350, 400, 550, 650};
    double disablePrices[4] = {338, 383, 539, 629};

    // 辅助函数
    int getDateType(const QString& date);
    QString getDateTypeString(int dateType);
};

#endif // QUERYDIALOG_H

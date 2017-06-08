#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <QFileDialog>
#include <QTranslator>
#include <QImage>
#include <cmath>
#include <QColor>
#include <QTableWidgetItem>
//#include <unistd.h>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_3_clicked();

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_tableWidget_itemChanged(QTableWidgetItem *item);

    void on_pushButton_4_clicked();

private:
    Ui::Widget *ui;
    QTranslator qtLanguageTranslator;
    QString file_name;
    double gKernel[99][99];
    QImage for_save;
    int iter = 0;
};

#endif // WIDGET_H

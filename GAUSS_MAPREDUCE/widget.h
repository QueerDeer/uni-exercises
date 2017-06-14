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
#include <unistd.h>
#include <QtConcurrent/QtConcurrent>
#include <thread>
#include <QThread>
#include <QPainter>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    double gKernel[99][99];

private slots:
    void on_pushButton_3_clicked();

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_tableWidget_itemChanged(QTableWidgetItem *item);

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::Widget *ui;
    QTranslator qtLanguageTranslator;
    QString file_name;
    QImage for_save;
    int kernel_size;
    int iter = 0;
    int w;
    int h;
    int flag;
};

#endif // WIDGET_H

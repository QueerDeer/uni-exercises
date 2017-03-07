#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <QFileDialog>
#include <QThread>
#include <QSemaphore>
#include <QFile>
#include <QTextStream>
#include <QDebug>

class Producer : public QThread
{
public:
    void run() override;
};

class Consumer : public QThread
{
    Q_OBJECT
public:
    void run() override;

signals:
    void stringConsumed(const QString &text);

protected:
    bool finish;
};

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
    void on_pushButton_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H

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
#include <QList>
#include "windows.h"

class Producer : public QThread
{
    Q_OBJECT
public:
    void run() override;

signals:
    void send(QList<QString>);
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
    void update(QList<QString> sendedlist);

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H

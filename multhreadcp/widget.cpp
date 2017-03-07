#include "widget.h"
#include "ui_widget.h"

int DataSize;
const int BufferSize = 64;
char buffer[BufferSize];
QString filename;
QSemaphore freeBytes(BufferSize);
QSemaphore usedBytes;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    filename = QFileDialog::getOpenFileName(this, "Open file", QDir::currentPath(), "text files (*.txt)");
    if(filename.isNull())
    {
        return;
    }
    ui->lineEdit->setText(filename);
    Producer producer;
    Consumer consumer;
    qRegisterMetaType<QList<QString>>("QList<QString>");
    connect(&producer, SIGNAL(send(QList<QString>)), this, SLOT(update(QList<QString>)));
    producer.start();
    consumer.start();
    producer.wait();
    consumer.wait();
}

void Widget::update (QList<QString> sendedlist)
{
    ui->lineEdit_4->setText(sendedlist.takeAt(0));
    ui->lineEdit_4->repaint();
    ui->lineEdit_5->setText(sendedlist.takeAt(0));
    ui->lineEdit_5->repaint();
    ui->lineEdit_2->setText(sendedlist.takeAt(0));
    ui->lineEdit_2->repaint();
    ui->lineEdit_3->setText(sendedlist.takeAt(0));
    ui->lineEdit_3->repaint();
    Sleep(64);
}



void Producer::run()
{
    QFile file_exp(filename);
    if( !file_exp.open(QFile::ReadOnly | QIODevice::Text) )
    {
        return;
    }
    DataSize = file_exp.size();
    QList<QString> sendlist;
    QString string;
    int realtimesize = 0;
    QTextStream in(&file_exp);
    for (int i = 0; i < DataSize; ++i)
    {
        freeBytes.acquire();
        in.operator >>(buffer[i % BufferSize]);
        realtimesize++;
        sendlist << QString::number(realtimesize);
        sendlist << QString::number(DataSize);
        sendlist << (string = QString::fromUtf8(buffer));
        sendlist << QString::number(realtimesize / BufferSize);
        emit send(sendlist);
        sendlist.clear();
        usedBytes.release();
    }
    file_exp.close();
}

void Consumer::run()
{
    QFile file_imp("txt_imp.txt");
    if( !file_imp.open(QFile::WriteOnly | QIODevice::Text) )
    {
        return;
    }
    QTextStream out(&file_imp);
    for (int i = 0; i < DataSize; ++i)
    {
        usedBytes.acquire();
        out.operator <<(buffer[i % BufferSize]);
        freeBytes.release();
    }
}

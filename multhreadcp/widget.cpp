#include "widget.h"
#include "ui_widget.h"

int DataSize = 128;
const int BufferSize = 8192;
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
    producer.start();
    consumer.start();
    producer.wait();
    consumer.wait();
}

void Producer::run()
{
    QFile file_exp(filename);
    if( !file_exp.open(QFile::ReadOnly | QIODevice::Text) )
    {
        return;
    }
    DataSize = file_exp.size();
    QTextStream in(&file_exp);
    for (int i = 0; i < DataSize; ++i)
    {
        freeBytes.acquire();
        in.operator >>(buffer[i % BufferSize]);
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

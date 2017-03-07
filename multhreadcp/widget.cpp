#include "widget.h"
#include "ui_widget.h"

const int DataSize = 100000;

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
    QFile file("txt.txt");
    for (int i = 0; i < DataSize; ++i)
    {
        freeBytes.acquire();
        //buffer[i % BufferSize] = "ACGT"[(int)qrand() % 4];
        usedBytes.release();
    }
}

void Consumer::run()
{
    for (int i = 0; i < DataSize; ++i) {
        usedBytes.acquire();
        //fprintf(stderr, "%c", buffer[i % BufferSize]);
        freeBytes.release();
    }
    //fprintf(stderr, "\n");
}

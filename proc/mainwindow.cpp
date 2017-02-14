#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextCodec>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    processname="";
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);

}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::on_actionFind_triggered()
{
    processname = QFileDialog::getOpenFileName(this, "Select", QDir::currentPath(), "executed files (*.exe)");
    if(processname=="")
    {
        return false;
    }
    else
    {
        ui ->label_3->setText(processname);
        ui->pushButton->setEnabled(true);
        return true;
    }
}

void MainWindow::on_pushButton_clicked()
{
    QString program = ui->label_3->text();
    QStringList arguments;
    arguments = ui->lineEdit->text().split(' ');
    m_time = QTime::currentTime();
    myProcess.start(program, arguments);
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(true);
    QObject::connect(&myProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(finished()));
    QObject::connect(&myProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readyReadStandardOutput()));
    ui->label_5->setText("");
    ui->label_7->setText("");
}

void MainWindow::on_pushButton_2_clicked()
{
    myProcess.terminate();
}

void MainWindow::finished()
{
    ui->label_5->setText(QString::number(myProcess.exitCode()));
    ui->label_7->setText(QString::number(m_time.elapsed())+ " ms");
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(false);
}

void MainWindow::readyReadStandardOutput()
{
    QByteArray buf = myProcess.readAllStandardOutput();
    QTextCodec *codec = QTextCodec::codecForName("IBM 866");
    QString str = codec->toUnicode(buf);
    ui->textBrowser->append(str);
}

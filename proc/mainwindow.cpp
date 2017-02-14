#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    processname="";
    ui->pushButton_2->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionFind_triggered()
{
    processname = QFileDialog::getOpenFileName(this, "Select", QDir::currentPath(), "executed files (*.exe)");
    ui->label_3->setText(processname);
}

bool MainWindow::on_pushButton_clicked()
{
    if(processname=="")
    {
        return false;
    }
    else
    {
        QString program = processname;
        QStringList arguments;
        arguments << ui->lineEdit->text();
        myProcess.start(program, arguments);
        ui->pushButton->setEnabled(false);
        ui->pushButton_2->setEnabled(true);

        QObject::connect(&myProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(finished(int,QProcess::ExitStatus)));
        QObject::connect(&myProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readyReadStandardOutput()));
        QObject::connect(&myProcess, SIGNAL(started()), this, SLOT(started()));


    }
}

void MainWindow::on_pushButton_2_clicked()
{
    myProcess.terminate();
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(false);
}

void MainWindow::error(QProcess::ProcessError error)
{
  outer = "Error: " + error;
  ui->textBrowser->setText(outer);
}

void 	MainWindow::finished(int exitCode, QProcess::ExitStatus exitStatus)
{
  outer = "Finished: " + exitCode;
  ui->textBrowser->setText(outer);
}

void 	MainWindow::readyReadStandardError()
{
  outer = "ReadyError";
  ui->textBrowser->setText(outer);
}

void 	MainWindow::readyReadStandardOutput()
{
  outer = "readyOut";
  QProcess *p = (QProcess *)sender();
  QByteArray buf = p->readAllStandardOutput();
  outer += buf;
  ui->textBrowser->setText(outer);
}

void 	MainWindow::started()
{
  outer = "Proc Started";
  ui->textBrowser->setText(outer);
}

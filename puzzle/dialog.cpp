#include <QString>
#include <QFile>
#include <QFileDialog>
#include <QtWidgets>
#include "puzzlesizewidget.h"
#include "dialog.h"
#include "ui_dialog.h"
#include "engine.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    filename = QFileDialog::getOpenFileName(this, "Open file", QDir::currentPath(), "image files (*.jpg *.jpeg *.png *.bmp *.ico)");
         if(filename.isNull())
         {
             return;
         }
         ui->lineEdit->setText(filename);
         ui->frame->setPixmap(QPixmap(filename));
         update();
}

void Dialog::on_buttonBox_rejected()
{
    this->close();

}

QString Dialog::imageFilePath()
{
    return ui->lineEdit->text();
}

QSize Dialog::puzzleSize()
{
    return ui->frame->value();
}

void Dialog::on_buttonBox_accepted()
{
   if(filename.isNull())
   {
       QMessageBox::warning(this,"Warning","There is no image to upload");
       QDialog::exec();
   }
   else
   {
   QGraphicsView* viewer=new QGraphicsView;
   Engine* scene=new Engine;
   scene->setup(puzzleSize(),QPixmap(imageFilePath()));
   viewer->setScene(scene);
   viewer->resize(700, 700);
   viewer->show();
   this->close();
   }
}

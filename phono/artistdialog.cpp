#include "artistdialog.h"
#include "ui_artistdialog.h"

ArtistDialog::ArtistDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ArtistDialog)
{
    ui->setupUi(this);
    m_mapper.setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    this->setWindowTitle("Artist's properties");
}

ArtistDialog::~ArtistDialog()
{
    delete ui;
}

void ArtistDialog::on_pushButton_clicked()
{
    fileName = QFileDialog::getOpenFileName(this, "Open image", QDir::currentPath(), "image files (*.jpg *.jpeg *.png *.bmp *.ico)");
    ui->label_3->setPixmap(QPixmap(fileName));
    update();
}

void ArtistDialog::setModel(musicmodel *model)
{
    m_mapper.setModel(model);
    m_mapper.addMapping(ui->lineEdit,0);
    m_mapper.toFirst();
}

void ArtistDialog::setModelIndex(const QModelIndex &index)
{
    QModelIndex parent = index.parent();
    m_mapper.setRootIndex(parent);
    m_mapper.setCurrentModelIndex(index);
    item = static_cast<Item*>(index.internalPointer());

    ui->lineEdit_2->setText(item->toArtist()->m_country);
    ui->lineEdit->setText(item->m_name);
    ui->plainTextEdit->setPlainText(item->m_comment);
    ui->label_3->setPixmap(item->toArtist()->m_photo);
}

void ArtistDialog::accept()
{
   m_mapper.submit();
   QDialog::accept();

   item->setComment(ui->plainTextEdit->toPlainText());
   item->toArtist()->setCountry(ui->lineEdit_2->text());
   item->toArtist()->setPhoto(*ui->label_3->pixmap());

}

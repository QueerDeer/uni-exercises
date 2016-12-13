#include "albumdialog.h"
#include "ui_albumdialog.h"

AlbumDialog::AlbumDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlbumDialog)
{
    ui->setupUi(this);
    m_mapper.setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    this->setWindowTitle("Album's properties");
}

AlbumDialog::~AlbumDialog()
{
    delete ui;
}

void AlbumDialog::on_pushButton_clicked()
{

    fileName = QFileDialog::getOpenFileName(this, "Open image", QDir::currentPath(), "image files (*.jpg *.jpeg *.png *.bmp *.ico)");
    ui->label_5->setPixmap(QPixmap(fileName));
    update();
}

void AlbumDialog::setModel(musicmodel *model)
{
    m_mapper.setModel(model);
    m_mapper.addMapping(ui->lineEdit, 0);
    m_mapper.toFirst();
}

void AlbumDialog::setModelIndex(const QModelIndex &index)
{
    QModelIndex parent = index.parent();
    m_mapper.setRootIndex(parent);
    m_mapper.setCurrentModelIndex(index);
    item = static_cast<Item*>(index.internalPointer());

    ui->label_6->setText(item->m_parent->m_name);
    ui->lineEdit->setText(item->m_name);
    ui->lineEdit_2->setText(item->toAlbum()->m_genre);
    ui->spinBox->setValue(item->toAlbum()->m_year);
    ui->plainTextEdit->setPlainText(item->m_comment);
    ui->label_5->setPixmap(item->toAlbum()->m_cover);
}

void AlbumDialog::accept()
{
   m_mapper.submit();
   QDialog::accept();

   item->setComment(ui->plainTextEdit->toPlainText());
   item->toAlbum()->setGenre(ui->lineEdit_2->text());
   item->toAlbum()->setYear(ui->spinBox->value());
   item->toAlbum()->setCover(*ui->label_5->pixmap());
}

bool AlbumDialog::addAlbum(musicmodel *model, const QModelIndex &parent)
{
    setModel(model);
    int row = model->rowCount(parent);

    if(!model->insertRow(row, parent))
        return false;

    QModelIndex index = model->index(row, 0, parent);
    setModelIndex(index);

    if(!exec())
    {
        model->removeRow(row, parent);
        return false;
    }

    return true;
}

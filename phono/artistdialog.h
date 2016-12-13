#ifndef ARTISTDIALOG_H
#define ARTISTDIALOG_H

#include "artist.h"
#include "album.h"
#include "song.h"
#include "musicmodel.h"
#include <QDialog>
#include <QFileDialog>
#include <QDataWidgetMapper>

namespace Ui {
class ArtistDialog;
}

class ArtistDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ArtistDialog(QWidget *parent = 0);
    ~ArtistDialog();
    void setModel(musicmodel *model);
    void setModelIndex(const QModelIndex &index);
    void accept();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ArtistDialog *ui;
    QString fileName;
    QDataWidgetMapper m_mapper;
    Item *item;
};

#endif // ARTISTDIALOG_H

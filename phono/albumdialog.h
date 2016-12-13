#ifndef ALBUMDIALOG_H
#define ALBUMDIALOG_H

#include "artist.h"
#include "album.h"
#include "song.h"
#include "musicmodel.h"
#include <QDialog>
#include <QFileDialog>
#include <QDataWidgetMapper>

namespace Ui {
class AlbumDialog;
}

class AlbumDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AlbumDialog(QWidget *parent = 0);
    ~AlbumDialog();
    void setModel(musicmodel *model);
    void setModelIndex(const QModelIndex &index);
    void accept();
    bool addAlbum(musicmodel *model, const QModelIndex &parent);

private slots:
    void on_pushButton_clicked();

private:
    Ui::AlbumDialog *ui;
    QString fileName;
    QDataWidgetMapper m_mapper;
    Item *item;
};

#endif // ALBUMDIALOG_H

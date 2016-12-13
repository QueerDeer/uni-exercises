#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QApplication::setApplicationName("Phonoteca");
    ui->setupUi(this);

    model = new musicmodel();
    ui->treeView->setModel(model);

    const int RatingColumn = 2;
    ratingdelegate *delegate = new ratingdelegate(ui->treeView);
    ui->treeView->setItemDelegateForColumn(RatingColumn, delegate);

    ui->treeView->setRootIndex(model->index(0,0,QModelIndex()));
    ui->actionProperties->setDisabled(true);
    ui->actionAdd_new_Artist->setEnabled(true);
    ui->actionAdd_new_Album->setDisabled(true);
    ui->actionAdd_new_Song->setDisabled(true);
    ui->actionDelete_Item->setDisabled(true);

    connect(ui->treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(positioning(QModelIndex)));
    connect(model, SIGNAL(dataChanged(QModelIndex, QModelIndex)), this, SLOT(documentModified()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::documentModified()
{
    setWindowModified(true);
}

void MainWindow::positioning(QModelIndex index)
{
    Item *item = static_cast<Item*>(index.internalPointer());

    if(item->toArtist())
    {
        ui->actionProperties->setEnabled(true);
        ui->actionAdd_new_Album->setEnabled(true);
        ui->actionAdd_new_Song->setDisabled(true);
        ui->actionDelete_Item->setEnabled(true);
    }

    if(item->toAlbum())
    {
        ui->actionProperties->setEnabled(true);
        ui->actionAdd_new_Album->setDisabled(true);
        ui->actionAdd_new_Song->setEnabled(true);
        ui->actionDelete_Item->setEnabled(true);
    }

    if(item->toSong())
    {
        ui->actionProperties->setDisabled(true);
        ui->actionAdd_new_Album->setDisabled(true);
        ui->actionAdd_new_Song->setDisabled(true);
        ui->actionDelete_Item->setEnabled(true);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(this->isWindowModified())
    {
        switch(QMessageBox::warning(this, tr("Document modified"), "The document has been modified. Do you want to save your changes?\nYou will lose any unsaved changes.", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Cancel))
        {

        case QMessageBox::Yes:
            if(saveFile())
            {
                    event->accept();
            } else
            {
                event->ignore();
            }
        break;

        case QMessageBox::No:
            event->accept();
            break;

        case QMessageBox::Cancel:
            event->ignore();
            break;
        }

    } else
    {
        event->accept();
    }
}

bool MainWindow::saveFile()
{
    if(file.isNull())
    {
        return saveFileAs();
    } else
    {
        QFile new_file(file);

        if(!new_file.open(QIODevice::WriteOnly))
        {
            QMessageBox::warning(this, "Error", "Can't save the file");
            return false;
        } else
        {
            QDataStream stream(&new_file);
            stream << *model->m_root;
            setWindowModified(false);
        }

        setWindowModified(false);
        setWindowTitle(QString("%1[*] -­ %2").arg(file.isNull()?"NewFile.txt":QFileInfo(file).fileName()).arg(QApplication::applicationName()));
        new_file.close();
        return true;
     }
}


bool MainWindow::saveFileAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save", file.isNull()?QDir::currentPath():file, "Text files(*.txt)");

    if(!fileName.isEmpty())
    {
        file=fileName;
        return saveFile();
    }
    return false;
}

void MainWindow::loadFile(const QString &fileName)
{
    delete model->m_root;
    model->m_root = new Item();
    QFile new_file(fileName);

    if(!new_file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, tr("Error"), tr("File could't be opened"));
        return;
    }

    file=fileName;
    QDataStream stream(&new_file);
    int buffer_artist;
    int buffer_album;
    int buffer_song;
    stream >> buffer_artist;

    for(int i=0; i<buffer_artist; ++i)
    {
        Artist *artist = new Artist();
        stream >> *artist;
        stream >> buffer_album;

        for(int j=0; j<buffer_album; ++j)
        {
            Album *album = new Album();
            stream >> *album;
            stream >> buffer_song;

            for(int k=0; k<buffer_song; ++k)
            {
                Song *song = new Song();
                stream >> *song;
                album->insertChild(song, k);
            }

            artist->insertChild(album, j);
        }

        model->m_root->insertChild(artist, i);
    }

    setWindowTitle(QString("%1[*] - ­%2").arg(file.isNull()?"NewFile.txt":QFileInfo(file).fileName()).arg(QApplication::applicationName()));
    new_file.close();
    ui->treeView->setModel(model);
    ui->treeView->reset();
}

void MainWindow::on_actionSave_triggered()
{
    saveFile();
}

void MainWindow::on_actionSave_As_triggered()
{
    saveFileAs();
}

void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open file", QDir::currentPath(), "text files (*.txt)");

    if(filename.isNull())
    {
        return;
    }

    if( file.isNull() && !isWindowModified())
    {
        loadFile(filename);
    } else
    {
        MainWindow *w = new MainWindow(0);
        w->loadFile(filename);
        w->show();
    }
}

void MainWindow::on_actionProperties_triggered()
{
    QModelIndex index = ui->treeView->selectionModel()->currentIndex();
    Item *item=static_cast<Item*>(index.internalPointer());
    if(item->toArtist())
    {
        ArtistDialog *artist = new ArtistDialog();
        artist->setModel(model);
        artist->setModelIndex(index);
        artist->show();
    }
    if(item->toAlbum())
    {
        AlbumDialog *album = new AlbumDialog();
        album->setModel(model);
        album->setModelIndex(index);
        album->show();
    }
}

void MainWindow::on_actionAdd_new_Artist_triggered()
{
    model->insertRows(model->m_root->childCount(), 1, QModelIndex());
    ArtistDialog *artist = new ArtistDialog();
    artist->setModel(model);
    artist->setModelIndex(model->index(model->m_root->childCount()-1 , 0, QModelIndex()));
    artist->show();
    setWindowModified(true);
}

void MainWindow::on_actionAdd_new_Album_triggered()
{
    QModelIndex index = ui->treeView->selectionModel()->currentIndex();
    Item *item = static_cast<Item*>(index.internalPointer());
    model->insertRows(item->childCount(), 1, index);
    AlbumDialog *album = new AlbumDialog();
    album->setModel(model);
    album->setModelIndex(model->index(item->childCount()-1,0,index));
    album->show();
    setWindowModified(true);
}

void MainWindow::on_actionAdd_new_Song_triggered()
{
    QModelIndex index = ui->treeView->selectionModel()->currentIndex();
    QAbstractItemModel *model = ui->treeView->model();

    if (model->columnCount(index) == 0)
        if (!model->insertColumn(0, index))
            return;
    if (!model->insertRow(index.row(), index))
        return;

    ui->treeView->selectionModel()->setCurrentIndex(model->index(0, 0, index), QItemSelectionModel::ClearAndSelect);
    setWindowModified(true);
}

void MainWindow::on_actionDelete_Item_triggered()
{
    QModelIndex index = ui->treeView->selectionModel()->currentIndex();
    QModelIndex parent = index.parent();
    Item *item = static_cast<Item*>(index.internalPointer());
    model->removeRows(item->m_parent->indexOf(item), 1, parent);

    if (model->rowCount() == 0)
    {
        ui->actionProperties->setDisabled(true);
        ui->actionAdd_new_Artist->setEnabled(true);
        ui->actionAdd_new_Album->setDisabled(true);
        ui->actionAdd_new_Song->setDisabled(true);
        ui->actionDelete_Item->setDisabled(true);
    }

    setWindowModified(true);
}

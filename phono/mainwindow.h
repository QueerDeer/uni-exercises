#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "artist.h"
#include "album.h"
#include "song.h"
#include "ratingdelegate.h"
#include <QTreeView>
#include "albumdialog.h"
#include "artistdialog.h"
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QCloseEvent>
#include <QTranslator>
#include <QLibraryInfo>
#include <QEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

signals:
    void editingFinished();

protected:
    void closeEvent(QCloseEvent *event);
    void changeEvent(QEvent * event) override;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void positioning(QModelIndex index);
    void documentModified();
    void loadFile(const QString &fileName);

private slots:
    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_actionOpen_triggered();

    void on_actionProperties_triggered();

    void on_actionAdd_new_Artist_triggered();

    void on_actionAdd_new_Album_triggered();

    void on_actionAdd_new_Song_triggered();

    void on_actionDelete_Item_triggered();

    void on_action_2_triggered();

    void on_actionEnglish_triggered();

private:
    Ui::MainWindow *ui;
    musicmodel *model;
    QString file;
    bool saveFile();
    bool saveFileAs();
    QTranslator qtLanguageTranslator;
};

#endif // MAINWINDOW_H

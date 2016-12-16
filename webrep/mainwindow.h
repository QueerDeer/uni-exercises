#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebView>
#include <QFileDialog>
#include <QDir>
#include <QWebElementCollection>
#include <QWebFrame>
#include <QTextCharFormat>
#include <QDate>
#include <QUrl>
#include <QMap>
#include "pluginfactory.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QWebFrame *frame;
    QMap<QDate, QWebElement> data;
    QWebElement temporary_seleceted;

private slots:
    void on_action_uploading_report_triggered();
    void loadFinish();
    void selectionChange();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

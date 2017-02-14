#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QStringList>
#include <QFileDialog>
#include <QDir>
#include <QTime>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QString processname;
    QProcess myProcess;
    QTime m_time;
    ~MainWindow();

private slots:
    bool on_actionFind_triggered();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void finished();

    void readyReadStandardOutput();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

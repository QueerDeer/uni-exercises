#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QStringList>
#include <QFileDialog>
#include <QDir>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QString processname;
    QString outer;
    QProcess myProcess;
    ~MainWindow();

private slots:
    void on_actionFind_triggered();

    bool on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void error(QProcess::ProcessError error);

    void finished(int exitCode, QProcess::ExitStatus exitStatus);

    void readyReadStandardError();

    void readyReadStandardOutput();

    void started();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

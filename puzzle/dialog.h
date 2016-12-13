#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    QString imageFilePath();
    QSize puzzleSize();

private slots:
    void on_pushButton_clicked();
    void on_buttonBox_rejected();
    void on_buttonBox_accepted();

private:
    Ui::Dialog *ui;
    QString filename;
};

#endif // DIALOG_H
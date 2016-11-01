#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QMainWindow>

namespace Ui {
class TextEditor;
}

class TextEditor : public QMainWindow
{
    Q_OBJECT

protected:

    void closeEvent(QCloseEvent *event);

public:
    explicit TextEditor(QWidget *parent = 0, const QString &fileName=QString());
    ~TextEditor();

private slots:
    void on_actionNew_triggered();

    void on_actionClose_triggered();

    void on_actionExit_triggered();

    void documentModified();

    void on_actionSelectFont_triggered();

    void on_actionAboutProgram_triggered();

    void on_actionAboutQt_triggered();

    void on_actionOpen_triggered();

    bool saveFile();

    bool saveFileAs();

    void on_actionAboutProgram_hovered();

private:
    Ui::TextEditor *ui;

    QString m_fileName;

    void loadFile(const QString &);

    void setFileName(const QString &fileName);

    QString getFileName(){return m_fileName;}

};

#endif // TEXTEDITOR_H

#include "texteditor.h"
#include "ui_texteditor.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QFontDialog>
#include <QSettings>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QFileInfo>


TextEditor::TextEditor(QWidget *parent, const QString &fileName) :
    QMainWindow(parent),
    ui(new Ui::TextEditor)
{
    ui->setupUi(this);
    QApplication::setOrganizationName("MepfiSoft");
    QApplication::setApplicationName("Text Editor");
    QSettings settings;
    QFont font = settings.value("ViewFont", QApplication::font()).value<QFont>();
    ui->textEdit->setFont(font);
}  

TextEditor::~TextEditor()
{
    delete ui;
}

void TextEditor::on_actionNew_triggered()
{
    TextEditor *w = new TextEditor;
    w->show();
}

void TextEditor::on_actionClose_triggered()
{
    this->close();
}

void TextEditor::on_actionExit_triggered()
{
    qApp->closeAllWindows();
}

void TextEditor::documentModified()
{
    setWindowModified(true);
}

void TextEditor::closeEvent(QCloseEvent *event)
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

void TextEditor::on_actionSelectFont_triggered()
{
    bool ok;
    QFont initial (QApplication::font());
    QFont font = QFontDialog::getFont(&ok, initial, this);
    if (ok)
    {
        ui->textEdit->setFont(font);
        QSettings fontsettings(QApplication::organizationName(), QApplication::applicationName());
        fontsettings.setValue("ViewFont", font);

    }
}

void TextEditor::on_actionAboutProgram_triggered()
{
    QMessageBox::about(this, tr("About me"), tr("Belozyorov Aleksander\nGroup b14-501(k05-331)"));
}

void TextEditor::on_actionAboutQt_triggered()
{
    QMessageBox::aboutQt(this, tr("About Qt:"));
}

void TextEditor::loadFile(const QString &fileName)
{
     if(fileName.isEmpty())
     {
         setFileName(QString());
         return;
     }
     QFile file(fileName);
     if( !file.open(QFile::ReadOnly | QIODevice::Text) )
     {
         QMessageBox::warning(this, tr("Error"), tr("File could't be opened"));
         return;
     }
     QTextStream text(&file);
     ui->textEdit->setText(text.readAll());
     setFileName(fileName);
     setWindowModified(false);
     file.close();
 }

 void TextEditor::setFileName(const QString &fileName)
 {
     m_fileName = fileName;
     setWindowTitle( QString("%1[*] - %2") .arg(m_fileName.isNull()?"untitled":QFileInfo(m_fileName).fileName()) .arg(QApplication::applicationName()) );
 }

 void TextEditor::on_actionOpen_triggered()
 {
     QString filename = QFileDialog::getOpenFileName(this, "Open file", QDir::currentPath(), "text files (*.txt)");
     if(filename.isNull())
     {
         return;
     }
     if( m_fileName.isNull() && !isWindowModified())
     {
         loadFile(filename);
     } else
     {
         TextEditor *w = new TextEditor(0, filename);
         w->show();
     }
 }

 bool TextEditor::saveFileAs()
 {
     QString filename = QFileDialog::getSaveFileName(this, "Save document", m_fileName.isNull()?QDir::currentPath():m_fileName, "text files (*.txt)");
     if(filename.isNull())
     {
         return false;
     }
     setFileName(filename);
     return saveFile();
 }

 bool TextEditor::saveFile()
 {
     if (m_fileName.isNull())
     {
         return saveFileAs();
     }
     QFile file(m_fileName);
     if(!file.open(QFile::WriteOnly | QIODevice::Text))
     {
         QMessageBox::warning(this, "Error", "Can't save the file");
         setFileName(QString());
         return false;
     }
     QTextStream text(&file);
     text << ui->textEdit->toPlainText();
     file.close();
     setWindowModified(false);
     return true;
 }

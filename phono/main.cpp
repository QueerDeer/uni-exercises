#include "mainwindow.h"
#include <QApplication>
#include <QTreeWidgetItem>
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator qtLanguageTranslator;
    qtLanguageTranslator.load(QString("QtLanguage_") + QString("ru"));
    qApp->installTranslator(&qtLanguageTranslator);

    MainWindow w;
    w.show();

    return a.exec();
}

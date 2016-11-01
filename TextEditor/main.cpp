#include "texteditor.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TextEditor w;
    w.show();
    w.move(320,110);

    return a.exec();
}

#include <QtWidgets/QApplication>
#include "global.hpp"
#include <qpushbutton.h>
#include "CZEditor.hpp"

int main(int argc, char *argv[])
{
    InitGlobals();
    QApplication a(argc, argv);
    cze = new CZEditor();
    return a.exec();
}

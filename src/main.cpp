#include <QtWidgets/QApplication>
#include "global.hpp"
#include <qpushbutton.h>
#include "CZEditor.hpp"

int main(int argc, char *argv[])
{
    InitGlobals();
    QApplication a(argc, argv);
    cze = new CZEditor();
    qWarning("%x", 0-(0x80000000>>31));
    return a.exec();
}

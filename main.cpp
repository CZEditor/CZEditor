#include "CZEditor.h"
#include <QtWidgets/QApplication>
#include "global.h"
#include <qpushbutton.h>

int main(int argc, char *argv[])
{
    InitGlobals();
    QApplication a(argc, argv);
    CZEditor w;
    
    return a.exec();
}

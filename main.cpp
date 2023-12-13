#include "CZEditor.h"
#include <QtWidgets/QApplication>
#include "global.h"

int main(int argc, char *argv[])
{
    InitGlobals();
    QApplication a(argc, argv);
    CZEditor w;
    w.show();
    return a.exec();
}

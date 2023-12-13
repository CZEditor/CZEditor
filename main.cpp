#include "CZEditor.hpp"
#include <QtWidgets/QApplication>
#include "global.hpp"

int main(int argc, char *argv[])
{
    InitGlobals();
    QApplication a(argc, argv);
    CZEditor w;
    w.show();
    return a.exec();
}

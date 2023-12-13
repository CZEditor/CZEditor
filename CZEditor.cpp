#include "CZEditor.h"
#include <QtWidgets/QSplitter>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QDockWidget>


CZEditor::CZEditor(QWidget *parent) : QMainWindow(parent)
{
    
    viewport = new CzeViewport(this);
    //char yeah[64];
    //snprintf(yeah, 64, "%i %i", viewport->width(), viewport->height());
    //new QLabel(yeah, this);
    //setCentralWidget(viewport);
}

CZEditor::~CZEditor()
{

}

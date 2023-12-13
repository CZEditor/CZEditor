#include "CZEditor.h"
#include <QtWidgets/QSplitter>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>


CZEditor::CZEditor(QWidget *parent) : CzeWindow(parent)
{
    SetTitle("CZEditor");
    QLineEdit* idk = new QLineEdit(this);
    idk->setGeometry(16, 16, 64, 64);
    viewport = new CzeViewport(nullptr);
    resize(200, 200);
    
    //char yeah[64];
    //snprintf(yeah, 64, "%i %i", viewport->width(), viewport->height());
    //new QLabel(yeah, this);
    //setCentralWidget(viewport);
}

CZEditor::~CZEditor()
{

}

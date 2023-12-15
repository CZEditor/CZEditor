#include "CZEditor.hpp"
#include <QtWidgets/QSplitter>
#include <QtWidgets/QLabel>
#include "CzeTextbox.hpp"
#include "CzeLabel.hpp"
#include "CzeButton.hpp"
#include "CzeTimeline.hpp"
#include "CzeProperties.hpp"

CZEditor::CZEditor(QWidget *parent) : CzeWindow(parent)
{
    SetTitle("CZEditor");
    CzeTextbox* idk = new CzeTextbox(this);
    idk->move(16,16);
    (new CzeLabel(this, "czeditor ! !"))->move(16, 48);
    (new CzeButton(this, "      !      ", [&]() {return;}))->move(96, 48);
    viewport = new CzeViewport(nullptr);
    new CzeTimeline(nullptr);
    new CzeProperties(nullptr);
    resize(200, 200);
    
    //char yeah[64];
    //snprintf(yeah, 64, "%i %i", viewport->width(), viewport->height());
    //new QLabel(yeah, this);
    //setCentralWidget(viewport);
}

CZEditor::~CZEditor()
{

}

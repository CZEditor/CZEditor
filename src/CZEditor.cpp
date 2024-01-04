#include "CZEditor.hpp"
#include <QtWidgets/QSplitter>
#include <QtWidgets/QLabel>
#include "CzeTextbox.hpp"
#include "CzeLabel.hpp"
#include "CzeButton.hpp"
#include "CzeTimeline.hpp"
#include "CzeProperties.hpp"
#include <qsplitter.h>
CZEditor::CZEditor(QWidget *parent) : CzeWindow(parent)
{
    SetTitle("CZEditor");
    //CzeTextbox* idk = new CzeTextbox(this);
    //idk->move(16,16);
    //(new CzeLabel(this, "czeditor ! !"))->move(16, 48);
    //(new CzeButton(this, "      !      "))->move(96, 48);
    viewport = new CzeViewport(nullptr);
    CzeTimeline* timeline = new CzeTimeline(nullptr);
    propertyWidget = new CzeProperties(nullptr);

    //viewport->inner->setParent(propertyWidget->splitter);
    //propertyWidget->splitter->addWidget(viewport->inner);
    //QSplitter* newq = new QSplitter(Qt::Vertical, propertyWidget->splitter);
    //propertyWidget->splitter->addWidget(newq);
    //propertyWidget->inner->setParent(newq);
    //timeline->inner->setParent(newq);
    //newq->addWidget(propertyWidget->inner);
    //newq->addWidget(timeline->inner);


    resize(200, 200);
    startTimer(33,Qt::PreciseTimer);
    setFocusPolicy(Qt::StrongFocus);
    //char yeah[64];
    //snprintf(yeah, 64, "%i %i", viewport->width(), viewport->height());
    //new QLabel(yeah, this);
    //setCentralWidget(viewport);
}

CZEditor::~CZEditor()
{

}

void CZEditor::SelectKeyframe(Keyframe* keyframe)
{
    propertyWidget->keyframe = keyframe;
    propertyWidget->UpdateParams();
}

void CZEditor::timerEvent(QTimerEvent* event)
{
    if (isplaying)
    {
        //qWarning("f");
        for (auto& keyframe : keyframelist.keyframes)
        {
            keyframe->source->checkForUpdate();
        }
        viewport->opengl->repaint();
        currentframe++;
    }
}

void CZEditor::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key::Key_Space)
    {
        isplaying = !isplaying;
    }
}
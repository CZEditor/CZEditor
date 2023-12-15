#include "CzeTimeline.hpp"
#include "CzeButton.hpp"
#include "CzeTextbox.hpp"
#include <QBoxLayout>
#include <qgraphicsscene.h>
#include <qgraphicsview.h>
#include <qgraphicsitem.h>

CzeTimeline::CzeTimeline(QWidget* parent) : CzeWindow(parent)
{
	SetTitle("Timeline");
	scene = new QGraphicsScene(this);
	opengl = new QGraphicsView(scene);
	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(opengl);
	opengl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	inner->setLayout(layout);
	(scene->addText("YIPPE", defaultfont))->setDefaultTextColor(QColor(255, 255, 255));
	scene->setBackgroundBrush(QColor(0, 0, 0));
}
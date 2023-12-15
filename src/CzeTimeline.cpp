#include "CzeTimeline.hpp"
#include "CzeButton.hpp"
#include "CzeTextbox.hpp"
#include <QBoxLayout>
#include <qgraphicsscene.h>
#include <qgraphicsview.h>
#include <qgraphicsitem.h>
#include "global.hpp"

class CzeTimelineKeyframeItem : public QGraphicsItem
{
public:
	CzeTimelineKeyframeItem(Keyframe* keyframeIn) : QGraphicsItem()
	{
		keyframe = keyframeIn;
	}

	QRectF boundingRect() const
	{
		return QRectF(-10.5, -10.5, 20, 20);
	}

	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr)
	{
		painter->setRenderHint(QPainter::Antialiasing);
		
		QRadialGradient grad(0, 10, 20);
		grad.setColorAt(0.0, GetAccentColor(255, 30));
		grad.setColorAt(0.5, GetAccentColor(255, 10));
		grad.setColorAt(0.55, GetAccentColor(16, 64));
		grad.setColorAt(0.7, GetAccentColor(8, 128));
		grad.setColorAt(1.0, GetAccentColor(0, 255));

		QLinearGradient maingrad(0, 10, 0, -10);
		maingrad.setColorAt(0.0, GetAccentColor(255, 30));
		maingrad.setColorAt(0.5, GetAccentColor(255, 10));
		maingrad.setColorAt(0.55, GetAccentColor(16, 64));
		maingrad.setColorAt(0.7, GetAccentColor(8, 128));
		maingrad.setColorAt(1.0, GetAccentColor(0, 255));

		painter->setPen(Qt::NoPen);
		painter->setBrush(maingrad);
		painter->drawRoundedRect(boundingRect(), 10,10);

		painter->setBrush(grad);
		
		painter->drawPie(boundingRect().x(), boundingRect().y(), 20, 20, 90 * 16, 180 * 16);

		grad.setCenter(boundingRect().right() - 10, 10);
		grad.setFocalPoint(boundingRect().right() - 10, 10);
		painter->setBrush(grad);
		painter->drawPie(boundingRect().right() - 20, boundingRect().y(), 20, 20, -90 * 16, 180 * 16);

		painter->setPen(QColor(255, 255, 255));
		painter->setBrush(Qt::NoBrush);
		painter->drawRoundedRect(boundingRect(), 10, 10);
	}

	Keyframe* keyframe;
};

class CzeTimelineView : public QGraphicsView
{
public:
	CzeTimelineView(QGraphicsScene* scene, QWidget* parent, KeyframeList* keyframelistIn) : QGraphicsView(scene, parent)
	{
		keyframelist = keyframelistIn;
	}

	void keyPressEvent(QKeyEvent* event)
	{
		if (event->text() == 'k')
		{
			Keyframe* newKeyframe = new Keyframe();
			keyframelist->keyframes.push_back(newKeyframe);
			scene()->addItem(new CzeTimelineKeyframeItem(newKeyframe));
		}
	}
	KeyframeList* keyframelist;
};

CzeTimeline::CzeTimeline(QWidget* parent) : CzeWindow(parent)
{
	SetTitle("Timeline");
	scene = new QGraphicsScene(this);
	opengl = new CzeTimelineView(scene,nullptr,&keyframelist);
	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(opengl);
	opengl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setLayout(layout);
	scene->setBackgroundBrush(QColor(0, 0, 0));
}
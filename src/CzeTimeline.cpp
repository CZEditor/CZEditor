#include "CzeTimeline.hpp"
#include "CzeButton.hpp"
#include "CzeTextbox.hpp"
#include <QBoxLayout>
#include <qgraphicsscene.h>
#include <qgraphicsview.h>
#include <qgraphicsitem.h>
#include "global.hpp"
#include <QGraphicsSceneMouseEvent>
#include <QTime>
#include "Properties.hpp"
#include "global.hpp"
#include "Effects.hpp"
#include "Actions.hpp"
#include "Sources.hpp"

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
		//sinf(((float)QTime::currentTime().msecsSinceStartOfDay())/1000)*30.0+32.0
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

		float cornerradius = fmin(10.0,boundingRect().width() / 2);

		if (cornerradius < 10.0)
		{
			grad.setCenter(boundingRect().center().x(), 10);
			grad.setFocalPoint(boundingRect().center().x(), 10);
			painter->setBrush(grad);
		}
		else
		{
			painter->setBrush(maingrad);
		}
		painter->setPen(Qt::NoPen);
		painter->drawRoundedRect(boundingRect(), cornerradius, cornerradius);

		if (boundingRect().width() >= boundingRect().height())
		{
			painter->setBrush(grad);

			painter->drawPie(boundingRect().x(), boundingRect().y(), 20, 20, 90 * 16, 180 * 16);

			grad.setCenter(boundingRect().right() - 10, 10);
			grad.setFocalPoint(boundingRect().right() - 10, 10);
			painter->setBrush(grad);
			painter->drawPie(boundingRect().right() - 20, boundingRect().y(), 20, 20, -90 * 16, 180 * 16);
		}

		painter->setPen(QColor(255, 255, 255));
		painter->setBrush(Qt::NoBrush);
		painter->drawRoundedRect(boundingRect(), cornerradius, cornerradius);
	}

	void mousePressEvent(QGraphicsSceneMouseEvent* event)
	{
		startdragpos = event->pos();
		cze->SelectKeyframe(keyframe);
		//qWarning("%llx ", keyframe->effects[0]->elements.hash_function()("vertexfunc"));
	}

	void mouseMoveEvent(QGraphicsSceneMouseEvent* event)
	{
		keyframe->frame = (event->scenePos() - startdragpos).x();
		UpdateVisual();
	}

	void UpdateVisual()
	{
		setPos(keyframe->frame, 0);
	}

	QPointF startdragpos;
	Keyframe* keyframe;
};

class CzeTimelineView : public QGraphicsView
{
public:
	CzeTimelineView(QGraphicsScene* scene, QWidget* parent = nullptr) : QGraphicsView(scene, parent)
	{
		playbackline = scene->addLine(0, -20, 0, 20, QColor(255, 255, 255));
	}

	void keyPressEvent(QKeyEvent* event)
	{
		if (event->text() == 'k')
		{
			Keyframe* newKeyframe = new Keyframe();
			newKeyframe->effects.push_back((Effect*)(EffectsDict["2D Media"](newKeyframe)));
			newKeyframe->effects.push_back(new Effects::SimpleImageEffect(newKeyframe));
			newKeyframe->actions.push_back(new Actions::NormalKeyframe(newKeyframe));
			newKeyframe->source = (Source*)(SourcesDict["Test"](newKeyframe));
			keyframelist.keyframes.push_back(newKeyframe);
			uninitializedKeyframes.push_back(newKeyframe);
			scene()->addItem(new CzeTimelineKeyframeItem(newKeyframe));
		}
	}
	
	void update()
	{
		playbackline->setPos(QPointF(currentframe, 0));
		QGraphicsView::update();
	}
	
	QGraphicsLineItem* playbackline;

};

CzeTimeline::CzeTimeline(QWidget* parent) : CzeWindow(parent)
{
	SetTitle("Timeline");
	scene = new QGraphicsScene(this);
	opengl = new CzeTimelineView(scene);
	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(opengl);
	opengl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setLayout(layout);
	scene->setBackgroundBrush(QColor(0, 0, 0));
	
}
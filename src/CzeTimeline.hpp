#pragma once

#include "CzeWindow.hpp"
#include <QGraphicsView>

class CzeTimeline : public CzeWindow
{
	Q_OBJECT

public:
	CzeTimeline(QWidget* parent);
	QGraphicsView* opengl;
	QGraphicsScene* scene;
};


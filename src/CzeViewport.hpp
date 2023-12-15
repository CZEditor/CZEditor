#pragma once

#include "CzeWindow.hpp"
#include <QGraphicsView>
#include <qopenglwidget.h>

class CzeViewport : public CzeWindow
{
	Q_OBJECT

public:
	CzeViewport(QWidget* parent);
	QOpenGLWidget* opengl;
};


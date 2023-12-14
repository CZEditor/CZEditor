#pragma once

#include "CzeWindow.hpp"
#include <QOpenGLWidget>

class CzeViewport : public CzeWindow
{
	Q_OBJECT

public:
	CzeViewport(QWidget* parent);
	QOpenGLWidget* opengl;
};


#include "CzeViewport.hpp"
#include "CzeButton.hpp"
#include "CzeTextbox.hpp"
#include <QBoxLayout>
#include <QOpenGLFunctions>
#include <qgraphicsscene.h>
#include <qgraphicsview.h>
#include <qgraphicsitem.h>
// this is for later
/*
class CzeViewportOpenGL : public QOpenGLWidget
{
public:
	CzeViewportOpenGL(QWidget* parent = nullptr) : QOpenGLWidget(parent)
	{

	}

	void initializeGL()
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthFunc(GL_LEQUAL);
	}

	void paintGL()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0, 0.0, 0.5, 1.0);
		glLoadIdentity();
	}


};
*/

CzeViewport::CzeViewport(QWidget* parent) : CzeWindow(parent)
{
	SetTitle("Viewport");
	scene = new QGraphicsScene(this);
	opengl = new QGraphicsView(scene);
	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(opengl);
	opengl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	inner->setLayout(layout);
	(scene->addText("YIPPE", defaultfont))->setDefaultTextColor(QColor(255,255,255));
	scene->setBackgroundBrush(QColor(0, 0, 0));
	
}
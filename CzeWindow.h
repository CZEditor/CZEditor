#pragma once
#include <QtWidgets/QWidget.h>
#include <QtGui/QEvent.h>
#include <QtGui/QPainter.h>
#include <time.h>
#include <QWindow>
#include "global.h"

class CzeWindow : public QWidget
{
	Q_OBJECT

public:
	CzeWindow(QWidget* parent);
	void resizeEvent(QResizeEvent* event)
	{
		resizehelper->setFixedSize(event->size());
		corners[0][0]->setGeometry(0, 0, 8, 8);
		corners[1][0]->setGeometry(8, 0, width() - 16, 2);
		corners[2][0]->setGeometry(width() - 16, 0, 8, 8);
		corners[0][1]->setGeometry(0, 8, 8, height() - 16);
		corners[2][1]->setGeometry(width() - 8, 8, 8, height() - 16);
		corners[0][2]->setGeometry(0, height() - 8, 8, 8);
		corners[1][2]->setGeometry(8, height() - 8, width() - 16, 8);
		corners[2][2]->setGeometry(width() - 8, height() - 8, 8, 8);
		QWidget::resizeEvent(event);
		titlebar->setGeometry(0, 0, event->size().width(), 24);
		closebutton->setGeometry(event->size().width() - closebutton->width() - 4, 4, closebutton->width(), closebutton->height());
	}
	void mousePressEvent(QMouseEvent* event)
	{
		Qt::Edges edg;
		if (event->pos().x() > width() - 9)
		{
			edg |= Qt::Edge::RightEdge;
		} 
		else if (event->pos().x() < 9)
		{
			edg |= Qt::Edge::LeftEdge;
		}
		if (event->pos().y() > height() - 9)
		{
			edg |= Qt::Edge::BottomEdge;
		}
		else if (event->pos().y() < 3)
		{
			edg |= Qt::Edge::TopEdge;
		}
		if(edg != 0)
		{ 
			windowHandle()->startSystemResize(edg);
		}
		else
		{
			if (event->pos().y() < 21)
			{
				windowHandle()->startSystemMove();
			}
		}
		QWidget::mousePressEvent(event);
	}
	void paintEvent(QPaintEvent* event)
	{
		QPainter qp(this);
		qp.setRenderHint(QPainter::Antialiasing);
		

		QRadialGradient grad(width() * 2, height() * 3, fmax(width(), height()) * 4);
		QColor col(255, 0, 0, 255);
		grad.setColorAt(0, GetAccentColor(30,100));
		grad.setColorAt(1, GetAccentColor(70,20));
		qp.setBrush(grad);
		qp.setPen(QPen(QColor(255, 255, 255),0));
		QRectF r = rect();
		r.setX(r.x() + 1.5);
		r.setY(r.y() + 1.5);
		r.setWidth(r.width() - 1.5);
		r.setHeight(r.height() - 1.5);
		qp.drawRoundedRect(r, 8, 8);
		qp.drawLine(QLineF(0, 24.5, width(), 24.5));
		qp.setBrush(Qt::NoBrush);
		qp.setPen(QPen(QColor(0, 0, 0), 0));
		qp.drawLine(QLineF(0, 25.5, width(), 25.5));
		

		r = rect();
		r.setX(r.x() + 0.5);
		r.setY(r.y() + 0.5);
		r.setWidth(r.width() - 0.5);
		r.setHeight(r.height() - 0.5);
		qp.drawRoundedRect(r, 8, 8);
		
	}
	QWidget* resizehelper;
	QWidget* titlebar;
	QWidget* corners[3][3];
	QWidget* closebutton;
};

#pragma once
#include <QtWidgets/QWidget>
#include <QtCore/QEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <time.h>
#include <QtGui/QWindow>
#include "global.hpp"
#include <QtWidgets/QLabel>

class CzeWindow : public QWidget
{
	Q_OBJECT

public:
	CzeWindow(QWidget* parent, const char* title = "");
	void resizeEvent(QResizeEvent* event)
	{
		if (!resizehelper)
		{
			QWidget::resizeEvent(event);
			return;
		}
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
		inner->setGeometry(0, 24, event->size().width(), event->size().height() - 24);
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
		grad.setColorAt(0, GetAccentColor(30,40));
		grad.setColorAt(1, GetAccentColor(70,10));
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

		if (title.length() == 0)
			return;

		qp.setRenderHint(QPainter::SmoothPixmapTransform);
		QFont fn("Segoe UI", 9);
		QFontMetrics fm(fn);
		QImage img(7, 7, QImage::Format::Format_RGBA8888);
		img.fill(QColor(255, 255, 255, 0));
		img.setPixel(3, 2, 0xcfffffff);
		img.setPixel(2, 2, 0xbfffffff);
		img.setPixel(4, 2, 0xbfffffff);
		QSize textsize = fm.size(Qt::TextSingleLine, title);
		qp.drawImage(QRect(6 - textsize.width() * 0.75, 4 - textsize.height()*1.5, textsize.width()*2.5, textsize.height() * 6), img);
		qp.setPen(QColor(0, 0, 0));
		qp.setFont(fn);
		qp.drawText(6, 18, title);
		
	}
	void childEvent(QChildEvent* event)
	{
		if (done && event->type() == QEvent::Type::ChildAdded)
		{
			if (event->child()->isWidgetType())
			{
				((QWidget*)event->child())->setParent(inner);
				((QWidget*)event->child())->show();
				return;
			}
		}
		else if (event->type() == QEvent::Type::ChildPolished)
		{
			if (event->child()->isWidgetType() && ((QWidget*)event->child())->isHidden())
			{
				((QWidget*)event->child())->show();
			}
		}
		QWidget::childEvent(event);
	}
	void SetTitle(const char* titlech)
	{
		title = title.fromUtf8(titlech, strlen(titlech));
	}
	bool done = false;
	QWidget* resizehelper = 0;
	QWidget* titlebar;
	QWidget* corners[3][3];
	QWidget* closebutton;
	QWidget* inner;
	QString title;
};

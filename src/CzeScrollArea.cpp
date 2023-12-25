#include "CzeScrollArea.hpp"
#include <qscrollbar.h>
#include <qpainter.h>
#include <algorithm>

class CzeScrollAreaViewport : public QWidget
{
public:
	void paintEvent(QPaintEvent* event)
	{

	}
};

CzeScrollBar::CzeScrollBar(QWidget* parent) : QWidget(parent)
{
	setFixedWidth(24);
	maximum = 1;
	value = 0;
}

void CzeScrollBar::paintEvent(QPaintEvent* event)
{
	if (height() >= maximum)
		return;
	float h = height();
	float offset = 0;
	QPainter qp(this);
	int size = maximum;
	float draggablesize = h / (float)size * h;
	qp.setBrush(QColor(0, 255, 0));
	qp.drawRect(0, (float)value / (float)maximum * (h - draggablesize), width(), draggablesize);
}

void CzeScrollBar::mousePressEvent(QMouseEvent* event)
{
	float draggablesize = (float)height() / (float)maximum * (float)height();
	float y = (float)value / (float)maximum * (height() - draggablesize);
	if (QRectF(0, y, width(), draggablesize).contains(event->pos()))
	{
		float draggablesize = (float)height() / (float)maximum * (float)height();
		float pos = (float)value / (float)maximum * (height() - draggablesize), width();
		draggingpos = event->pos();
		draggingpos.setY(draggingpos.y() - pos);
		dragging = true;
	}
	else
	{
		dragging = false;
	}
}

void CzeScrollBar::mouseMoveEvent(QMouseEvent* event)
{
	if (dragging)
	{
		float draggablesize = (float)height() / (float)maximum * (float)height();
		value = std::clamp((int)((event->pos().y()-draggingpos.y()) / (height() - draggablesize) * maximum),0,maximum);
		emit valueChanged(value);
		repaint();
	}
}

CzeScrollArea::CzeScrollArea(QWidget* parent) : QWidget(parent)
{
	inner = new QWidget(this);
	verticalScrollBar = new CzeScrollBar(this);
	connect(verticalScrollBar, &CzeScrollBar::valueChanged, this, &CzeScrollArea::onScrollChanged);
	done = true;
}

void CzeScrollArea::paintEvent(QPaintEvent* event)
{
	
}

void CzeScrollArea::onScrollChanged(int value)
{
	inner->move(0, -(value)*(((float)inner->height() - (float)height()) /(float)inner->height()));
}
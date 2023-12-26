#pragma once

#include <QWidget>
#include <QPainter>
#include <qevent.h>
#include <qmath.h>

class CzeColorPicker : public QWidget
{
	Q_OBJECT
public:
	CzeColorPicker(QWidget* parent);
	void paintEvent(QPaintEvent* event);
	QSize sizeHint()
	{
		return QSize(360, 256);
	}
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

	void updateColor(QPoint pos);

	bool changingHue = false;
	QPixmap* pix = 0;
	QColor color;
signals:
	void colorChanged(QColor color);
};
#pragma once

#include <QWidget>
#include <QPainter>
#include <qevent.h>
#include <qmath.h>
#include "CzeSpinBox.hpp"


class CzeColorGradient : public QWidget
{
	Q_OBJECT
public:
	CzeColorGradient(QWidget* parent);

	void paintEvent(QPaintEvent* event);

	void mousePressEvent(QMouseEvent* event);

	void mouseMoveEvent(QMouseEvent* event);

	void mouseReleaseEvent(QMouseEvent* event);


	void updateColor(QPoint pos);

	void changeColor(QColor colorIn);
	

	QSize sizeHint();

	bool changingHue = false;
	QPixmap* pix = 0;
	QColor color;


signals:
	void colorChanged(QColor color);
};

class CzeColorPicker : public QWidget
{
	Q_OBJECT
public:
	CzeColorPicker(QWidget* parent);

	void colorChangedSlot(QColor color);

	CzeSpinBox* red;
	CzeSpinBox* green;
	CzeSpinBox* blue;
	CzeSpinBox* alpha;
	bool block = false;

	void redChanged(int value);
	void greenChanged(int value);
	void blueChanged(int value);
	void alphaChanged(int value);
	QColor color;
	CzeColorGradient* gradient;

signals:
	void colorChanged(QColor color);
};
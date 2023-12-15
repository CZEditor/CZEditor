#pragma once
#include <QtWidgets/QAbstractButton>
#include <functional>
#include <QtCore/QEvent>
#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
#include "global.hpp"

class CzeButton : public QAbstractButton
{
	Q_OBJECT

public:
	CzeButton(QWidget* parent, const char* text, std::function<void()> callbackIn, void* argsIn = nullptr);
	//void pressedEvent()
	//{
	//	callback();
	//}
	void enterEvent(QEnterEvent* event)
	{
		hovered = true;
		repaint();
		QWidget::enterEvent(event);
	}

	void leaveEvent(QEvent* event)
	{
		hovered = false;
		repaint();
		QWidget::leaveEvent(event);
	}

	void mousePressEvent(QMouseEvent* event)
	{
		held = true;
		repaint();
		callback();
		QWidget::mousePressEvent(event);
	}

	void mouseReleaseEvent(QMouseEvent* event)
	{
		held = false;
		repaint();
		QWidget::mouseReleaseEvent(event);
	}



	

	void paintEvent(QPaintEvent* event)
	{
		QPainter qp(this);
		QRadialGradient grad(width() / 2, fmax(height(), width()) * 4, fmax(height(),width()) * 8-height()*1);
		qp.setRenderHint(QPainter::Antialiasing);
		qp.setRenderHint(QPainter::TextAntialiasing);
		float aspect = ((float)height() / (float)width());
		if (held)
		{
			grad.setColorAt(0.5 - 0.06 * (aspect), GetAccentColor(255, 192));
			grad.setColorAt(0.5 - 0.02 * (aspect), GetAccentColor(255, 64));
			grad.setColorAt(0.5, GetAccentColor(255, 32));
			grad.setColorAt(0.5 + 0.04 * (aspect), GetAccentColor(96, 64));
			grad.setColorAt(0.5 + 0.06 * (aspect), GetAccentColor(96, 128));
			qp.setPen(QColor(255, 255, 255));
			qp.setBrush(grad);
		}
		else if (hovered)
		{
			grad.setColorAt(0.5 - 0.06*(aspect), GetAccentColor(192, 255));
			grad.setColorAt(0.5 - 0.02 * (aspect), GetAccentColor(255, 128));
			grad.setColorAt(0.5, GetAccentColor(255, 96));
			grad.setColorAt(0.5 + 0.04 * (aspect), GetAccentColor(50, 192));
			grad.setColorAt(0.5 + 0.06 * (aspect), GetAccentColor(10, 255));
			qp.setPen(QColor(255, 255, 255));
			qp.setBrush(grad);
		}
		else
		{
			//grad.setColorAt(0.3, GetAccentColor(192, 255));
			grad.setColorAt(0.5 - 0.02 * (aspect), GetAccentColor(32, 32));
			grad.setColorAt(0.5, GetAccentColor(32, 16));
			grad.setColorAt(0.5 + 0.02 * (aspect), GetAccentColor(12, 64));
			grad.setColorAt(0.5 + 0.06 * (aspect), GetAccentColor(12, 192));
			qp.setPen(QColor(255, 255, 255));
			qp.setBrush(grad);

		}

		qp.drawRoundedRect(QRectF(1.5, 1.5, ((float)width()) - 3.0, ((float)height()) - 3.0), 4, 4);
		qp.setBrush(Qt::NoBrush);
		qp.setPen(QColor(0, 0, 0));
		qp.drawRoundedRect(QRectF(0.5, 0.5, ((float)width()) - 1.0, ((float)height()) - 1.0), 4, 4);

		QFont fn("Segoe UI", 9);
		QFontMetrics fm(fn);
		qp.setPen(QColor(255, 255, 255));
		qp.setFont(fn);
		qp.drawText(4,fm.height(), text());

		event->accept();
	}

	std::function<void()> callback;
	void* args;

	bool hovered = false;
	bool held = false;
};


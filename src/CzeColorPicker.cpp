#include "CzeColorPicker.hpp"

CzeColorPicker::CzeColorPicker(QWidget* parent) : QWidget(parent)
{
	color = QColor(255, 0, 0);
	setFixedSize(180+16, 180);
}

void CzeColorPicker::paintEvent(QPaintEvent* event)
{
	QPainter qp(this);
	int w = width()-16;
	int h = height();
	QLinearGradient grad(0, 0, 0, h);
	if (!pix || pix->width() != w || pix->height() != h)
	{
		delete pix;
		QImage img(w, h, QImage::Format_RGB32);
		unsigned int* bits = (unsigned int*)img.bits();
		int scanline = w;
		for (int x = 0; x < w; ++x)
		{
			int i = x;
			int hue = (x*360)/w;
			for (int y = 0; y < h; ++y)
			{
				bits[i] = QColor::fromHsv(hue, 255, 255).rgb();
				i += scanline;
			}
		}
		pix = new QPixmap(QPixmap::fromImage(img));
		QPainter lmao(pix);
		grad.setColorAt(0.0, QColor(255, 255, 255, 0));
		grad.setColorAt(1.0, QColor(255, 255, 255, 255));
		lmao.setBrush(grad);
		lmao.drawRect(0, 0, w, h);
	}
	qp.drawPixmap(0, 0, *pix);
	qp.setPen(QColor(255, 255, 255));
	int x = (color.hue() * w) / 360;
	int y = h - (color.hsvSaturation() * h) / 255;
	qp.drawEllipse(QPoint(x, y), 3, 3);
	qp.setPen(QColor(0, 0, 0));
	qp.drawEllipse(QPoint(x, y), 4, 4);
	qp.setBrush(QColor::fromHsv(color.hue(),color.saturation(),255));
	qp.drawRect(w, 0, 15, h-1);
	grad.setColorAt(0.0, QColor(0, 0, 0, 0));
	grad.setColorAt(1.0, QColor(0, 0, 0, 255));
	qp.setBrush(grad);
	qp.setPen(Qt::NoPen);
	qp.drawRect(w+1, 1, 14, h-2);
	qp.setBrush(Qt::NoBrush);
	y = h - (color.value() * h) / 255;
	qp.setPen(QColor(255,255,255));
	qp.drawRect(w+1,y-2,13,4);
	qp.setPen(QColor(0, 0, 0));
	qp.drawRect(w, y - 3, 15, 6);
}

void CzeColorPicker::mousePressEvent(QMouseEvent* event)
{
	changingHue = event->pos().x() < width() - 16;
}

void CzeColorPicker::mouseMoveEvent(QMouseEvent* event)
{
	updateColor(event->pos());
}

void CzeColorPicker::mouseReleaseEvent(QMouseEvent* event)
{
	updateColor(event->pos());
}


void CzeColorPicker::updateColor(QPoint pos)
{
	if (changingHue)
	{
		color = QColor::fromHsv(std::clamp(pos.x() * 360 / (width() - 16), 0, 359), std::clamp(255 - pos.y() * 255 / height(), 0, 255), color.value());
	}
	else
	{
		color = QColor::fromHsv(color.hue(), color.hsvSaturation(), std::clamp(255 - pos.y() * 255 / height(), 0, 255));
	}
	emit colorChanged(color);
	repaint();
}
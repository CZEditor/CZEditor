#include "CzeColorPicker.hpp"
#include "CzeLabel.hpp"

CzeColorGradient::CzeColorGradient(QWidget* parent) : QWidget(parent)
{
	color = QColor(255, 0, 0);
	setFixedSize(180 + 16, 180);
}

void CzeColorGradient::paintEvent(QPaintEvent* event)
{
	QPainter qp(this);
	int w = width() - 16;
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
			int hue = (x * 360) / w;
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
	qp.setBrush(QColor::fromHsv(color.hue(), color.saturation(), 255));
	qp.drawRect(w, 0, 15, h - 1);
	grad.setColorAt(0.0, QColor(0, 0, 0, 0));
	grad.setColorAt(1.0, QColor(0, 0, 0, 255));
	qp.setBrush(grad);
	qp.setPen(Qt::NoPen);
	qp.drawRect(w + 1, 1, 14, h - 2);
	qp.setBrush(Qt::NoBrush);
	y = h - (color.value() * h) / 255;
	qp.setPen(QColor(255, 255, 255));
	qp.drawRect(w + 1, y - 2, 13, 4);
	qp.setPen(QColor(0, 0, 0));
	qp.drawRect(w, y - 3, 15, 6);
}

void CzeColorGradient::mousePressEvent(QMouseEvent* event)
{
	changingHue = event->pos().x() < width() - 16;
}

void CzeColorGradient::mouseMoveEvent(QMouseEvent* event)
{
	updateColor(event->pos());
}

void CzeColorGradient::mouseReleaseEvent(QMouseEvent* event)
{
	updateColor(event->pos());
}


void CzeColorGradient::updateColor(QPoint pos)
{
	if (changingHue)
	{
		color = QColor::fromHsv(std::clamp(pos.x() * 360 / (width() - 16), 0, 359), std::clamp(255 - pos.y() * 255 / height(), 0, 255), color.value(), color.alpha());
	}
	else
	{
		color = QColor::fromHsv(color.hue(), color.hsvSaturation(), std::clamp(255 - pos.y() * 255 / height(), 0, 255), color.alpha());
	}
	emit colorChanged(color);
	repaint();
}

void CzeColorGradient::changeColor(QColor colorIn)
{
	color = colorIn;
	repaint();
}

QSize CzeColorGradient::sizeHint()
{
	return QSize(180+16, 180);
}






CzeColorPicker::CzeColorPicker(QWidget* parent) : QWidget(parent)
{
	gradient = new CzeColorGradient(this);
	connect(gradient, &CzeColorGradient::colorChanged, this, &CzeColorPicker::colorChangedSlot);
	setFixedSize(180 + 16 + 64 + 32,180);
	red = new CzeSpinBox(this);
	red->setGeometry(180 + 32, 0, 64, 24);
	red->setRange(0, 255);
	green = new CzeSpinBox(this);
	green->setGeometry(180 + 32, 24+4, 64, 24);
	green->setRange(0, 255);
	blue = new CzeSpinBox(this);
	blue->setGeometry(180 + 32, 24 + 4 + 24 + 4, 64, 24);
	blue->setRange(0, 255);
	alpha = new CzeSpinBox(this);
	alpha->setGeometry(180 + 32, 24 + 4 + 24 + 4 + 24 + 4, 64, 24);
	alpha->setRange(0, 255);
	(new CzeLabel(this, "R"))->setGeometry(180 + 32 + 64 + 4, 0, 16, 24);
	(new CzeLabel(this, "G"))->setGeometry(180 + 32 + 64 + 4, 24 + 4, 16, 24);
	(new CzeLabel(this, "B"))->setGeometry(180 + 32 + 64 + 4, 24 + 4 + 24 + 4, 16, 24);
	(new CzeLabel(this, "A"))->setGeometry(180 + 32 + 64 + 4, 24 + 4 + 24 + 4 + 24 + 4, 16, 24);
	connect(red, &CzeSpinBox::valueChanged, this, &CzeColorPicker::redChanged);
	connect(green, &CzeSpinBox::valueChanged, this, &CzeColorPicker::greenChanged);
	connect(blue , &CzeSpinBox::valueChanged, this, &CzeColorPicker::blueChanged);
	connect(alpha, &CzeSpinBox::valueChanged, this, &CzeColorPicker::alphaChanged);
}

void CzeColorPicker::redChanged(int value)
{
	if (block)
	{
		return;
	}
	color.setRed(value);
	gradient->changeColor(color);
	emit colorChanged(color);
}

void CzeColorPicker::greenChanged(int value)
{
	if (block)
	{
		return;
	}
	color.setGreen(value);
	gradient->changeColor(color);
	emit colorChanged(color);
}

void CzeColorPicker::blueChanged(int value)
{
	if (block)
	{
		return;
	}
	color.setBlue(value);
	gradient->changeColor(color);
	emit colorChanged(color);
}

void CzeColorPicker::alphaChanged(int value)
{
	if (block)
	{
		return;
	}
	color.setAlpha(value);
	gradient->changeColor(color);
	emit colorChanged(color);
}

void CzeColorPicker::colorChangedSlot(QColor colorIn)
{
	emit colorChanged(colorIn);
	color = colorIn;
	block = true;
	red->setValue(color.red());
	green->setValue(color.green());
	blue->setValue(color.blue());
	alpha ->setValue(color.alpha());
	block = false;
}

#include "CzeColorPicker.moc"
#include <QColorDialog>
#include "PropertyWidgets.hpp"
#include <qsizepolicy.h>
#include "Properties.hpp"
#include <qformlayout.h>
#include "CzeLabel.hpp"



IntPropertyWidget::IntPropertyWidget(IntProperty* propIn, QWidget* parent) : QWidget(parent)
{
	textbox = new CzeSpinBox(this);
	textbox->setValue(propIn->data.data);
	connect(textbox, &QSpinBox::valueChanged, this, &IntPropertyWidget::textchanged);
	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	setMinimumWidth(68);
	setMinimumHeight(24);
	prop = propIn;
}

void IntPropertyWidget::textchanged(int value)
{
	prop->data.data = value;
}

VertexPropertyWidget::VertexPropertyWidget(VertexProperty* propIn, QWidget* parent) : QWidget(parent)
{
	prop = propIn;
	QFormLayout* l = new QFormLayout(this);
	for (int i = 0; i < 3; i++)
	{
		l->addRow(new CzeLabel(this, QString::asprintf("Vert%i X", i)), (values[i][0] = new CzeSpinBox(this)));
		connect(values[i][0], &CzeSpinBox::valueChanged, this, [&, i](float g) {textchanged(g, i * 3); });
		values[i][0]->setValue(propIn->vertices[i].x());
		l->addRow(new CzeLabel(this,QString::asprintf("Vert%i Y",i)), (values[i][1] = new CzeSpinBox(this)));
		connect(values[i][1], &CzeSpinBox::valueChanged, this, [&, i](float g) {textchanged(g, i * 3 + 1); });
		values[i][1]->setValue(propIn->vertices[i].y());
		l->addRow(new CzeLabel(this,QString::asprintf("Vert%i Z",i)), (values[i][2] = new CzeSpinBox(this)));
		connect(values[i][2], &CzeSpinBox::valueChanged, this, [&, i](float g) {textchanged(g, i * 3 + 2); });
		values[i][2]->setValue(propIn->vertices[i].z());
	}
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void VertexPropertyWidget::textchanged(float value, int i)
{
	prop->vertices[i / 3][i % 3] = value;
}

ColorPropertyWidget::ColorPropertyWidget(ColorProperty* propIn, QWidget* parent) : QWidget(parent)
{
	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	setMinimumWidth(64);
	setMinimumHeight(24);

	prop = propIn;
}

void ColorPropertyWidget::paintEvent(QPaintEvent* event)
{
	QPainter qp(this);
	qp.setRenderHint(QPainter::Antialiasing);
	
	//qp.setPen(QColor(255, 255, 255));
	//qp.setBrush(prop->color);

	//QRectF r = rect();
	//qp.drawRect(QRectF(r.x() + 1.5, r.y() + 1.5, r.width() - 4, r.height() - 4));

	QRadialGradient grad(12, height(), height());
	grad.setColorAt(0.0, InterpolateQColor(prop->color,QColor(255,255,255),0.5));
	grad.setColorAt(0.3, prop->color);
	grad.setColorAt(0.55, prop->color);
	grad.setColorAt(0.7, InterpolateQColor(prop->color, QColor(255, 255, 255), 0.5));
	grad.setColorAt(1.0, InterpolateQColor(prop->color, QColor(255, 255, 255), 0.75));

	QLinearGradient maingrad(0, height(), 0, 0);
	maingrad.setColorAt(0.0, InterpolateQColor(prop->color, QColor(255, 255, 255), 0.5));
	maingrad.setColorAt(0.3, prop->color);
	maingrad.setColorAt(0.55, prop->color);
	maingrad.setColorAt(0.7, InterpolateQColor(prop->color, QColor(255, 255, 255), 0.5));
	maingrad.setColorAt(1.0, InterpolateQColor(prop->color, QColor(255, 255, 255), 0.75));

	float cornerradius = fmin(12.0, width() / 2);

	if (cornerradius < 12.0)
	{
		grad.setCenter(rect().center().x(), 24);
		grad.setFocalPoint(rect().center().x(), 24);
		qp.setBrush(grad);
	}
	else
	{
		qp.setBrush(maingrad);
	}
	qp.setPen(Qt::NoPen);
	qp.drawRoundedRect(rect(), cornerradius, cornerradius);

	if (width() >= height())
	{
		qp.setBrush(grad);

		qp.drawPie(0, 0, 24, 24, 90 * 16, 180 * 16);

		grad.setCenter(rect().right() - 12, 24);
		grad.setFocalPoint(rect().right() - 12, 24);
		qp.setBrush(grad);
		qp.drawPie(rect().right() - 24, 0, 24, 24, -90 * 16, 180 * 16);
	}

	qp.setPen(QColor(255, 255, 255));
	qp.setBrush(Qt::NoBrush);
	qp.drawRoundedRect(rect(), cornerradius, cornerradius);


}

void ColorPropertyWidget::mousePressEvent(QMouseEvent* event)
{
	QColorDialog* dialog = new QColorDialog(prop->color, this);
	connect(dialog, &QColorDialog::colorSelected, this, &ColorPropertyWidget::colorSelected);
	dialog->show();
}

void ColorPropertyWidget::colorSelected(const QColor& color)
{
	prop->color = color;
	repaint();
}


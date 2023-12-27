#include <QColorDialog>
#include "PropertyWidgets.hpp"
#include <qsizepolicy.h>
#include "Properties.hpp"
#include <qformlayout.h>
#include "CzeLabel.hpp"
#include "global.hpp"
#include "CzeColorPicker.hpp"


IntPropertyWidget::IntPropertyWidget(IntProperty* propIn, QWidget* parent) : QWidget(parent)
{
	textbox = new CzeSpinBox(this);
	textbox->setValue(propIn->data.data);
	connect(textbox, &QSpinBox::valueChanged, this, &IntPropertyWidget::textchanged);
	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	setMinimumWidth(64);
	setMinimumHeight(24);
	prop = propIn;
}

void IntPropertyWidget::textchanged(int value)
{
	prop->data.data = value;
	if (prop->callback)
	{
		prop->callback(prop->callbackData);
	}
}

VertexPropertyWidget::VertexPropertyWidget(VertexProperty* propIn, QWidget* parent) : QWidget(parent)
{
	prop = propIn;
	QFormLayout* l = new QFormLayout(this);
	for (int i = 0; i < 3; i++)
	{
		l->addRow(new CzeLabel(this, QString::asprintf("Vert%i X", i)), (values[i][0] = new CzeDoubleSpinBox(this)));
		connect(values[i][0], &CzeDoubleSpinBox::valueChanged, this, [&, i](float g) {textchanged(g, i * 3); });
		values[i][0]->setValue(propIn->vertices[i].x());
		l->addRow(new CzeLabel(this,QString::asprintf("Vert%i Y",i)), (values[i][1] = new CzeDoubleSpinBox(this)));
		connect(values[i][1], &CzeDoubleSpinBox::valueChanged, this, [&, i](float g) {textchanged(g, i * 3 + 1); });
		values[i][1]->setValue(propIn->vertices[i].y());
		l->addRow(new CzeLabel(this,QString::asprintf("Vert%i Z",i)), (values[i][2] = new CzeDoubleSpinBox(this)));
		connect(values[i][2], &CzeDoubleSpinBox::valueChanged, this, [&, i](float g) {textchanged(g, i * 3 + 2); });
		values[i][2]->setValue(propIn->vertices[i].z());
	}
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void VertexPropertyWidget::textchanged(float value, int i)
{
	prop->vertices[i / 3][i % 3] = value;
	if (prop->callback)
	{
		prop->callback(prop->callbackData);
	}
}

ColorPropertyWidget::ColorPropertyWidget(ColorProperty* propIn, QWidget* parent) : QWidget(parent)
{
	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	CzeColorPicker* picker = new CzeColorPicker(this);
	connect(picker, &CzeColorPicker::colorChanged, this, &ColorPropertyWidget::colorSelected);
	setMinimumWidth(360);
	setMinimumHeight(256);

	prop = propIn;
}


/*
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


*/
void ColorPropertyWidget::colorSelected(QColor color)
{
	prop->color = color;
	if (prop->callback)
	{
		prop->callback(prop->callbackData);
	}
}

StringPropertyWidget::StringPropertyWidget(StringProperty* propIn, QWidget* parent) : QWidget(parent)
{
	textbox = new CzeTextbox(this);
	textbox->setText(propIn->text);
	connect(textbox, &CzeTextbox::textChanged, this, &StringPropertyWidget::textchanged);
	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	setMinimumWidth(64);
	setMinimumHeight(24);
	prop = propIn;
}

void StringPropertyWidget::textchanged(QString text)
{
	prop->text = text;
	if (prop->callback)
	{
		prop->callback(prop->callbackData);
	}
}

QuadPropertyWidget::QuadPropertyWidget(QuadProperty* propIn, QWidget* parent) : QWidget(parent)
{
	prop = propIn;
	QFormLayout* l = new QFormLayout(this);
	for (int i = 0; i < 4; i++)
	{
		l->addRow(new CzeLabel(this, QString::asprintf("Vert%i X", i)), (values[i][0] = new CzeDoubleSpinBox(this)));
		connect(values[i][0], &CzeDoubleSpinBox::valueChanged, this, [&, i](float g) {textchanged(g, i * 3); });
		values[i][0]->setValue(propIn->vertices[i].x());
		l->addRow(new CzeLabel(this, QString::asprintf("Vert%i Y", i)), (values[i][1] = new CzeDoubleSpinBox(this)));
		connect(values[i][1], &CzeDoubleSpinBox::valueChanged, this, [&, i](float g) {textchanged(g, i * 3 + 1); });
		values[i][1]->setValue(propIn->vertices[i].y());
		l->addRow(new CzeLabel(this, QString::asprintf("Vert%i Z", i)), (values[i][2] = new CzeDoubleSpinBox(this)));
		connect(values[i][2], &CzeDoubleSpinBox::valueChanged, this, [&, i](float g) {textchanged(g, i * 3 + 2); });
		values[i][2]->setValue(propIn->vertices[i].z());
	}
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void QuadPropertyWidget::textchanged(float value, int i)
{
	prop->vertices[i / 3][i % 3] = value;
	if (prop->callback)
	{
		prop->callback(prop->callbackData);
	}
}

OriginPropertyWidget::OriginPropertyWidget(OriginProperty* propIn, QWidget* parent) : QWidget(parent)
{
	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

	originpresetgridlayout = new QGridLayout(this);
	originpresetgridlayout->setSpacing(0);

	for (int i = 0; i < 9; i++)
	{
		originpresetbuttons[i] = new CzeButton(this, buttonlabels[i], *(void **)&i);
		originpresetbuttons[i]->setMinimumSize(24, 24);

		connect(originpresetbuttons[i], &CzeButton::pressed, this, &OriginPropertyWidget::originPresetPressed);

		originpresetgridlayout->addWidget(originpresetbuttons[i], i / 3, i % 3);
	}

	for (int i = 0; i < 2; i++)
	{
		originspinboxes[i] = new CzeDoubleSpinBox(this);
		originspinboxes[i]->setRange(0.0f, 1.0f);
		originspinboxes[i]->setSingleStep(0.1f);

		originpresetgridlayout->addWidget(originspinboxes[i], i * 2, 4);
	}

	connect(originspinboxes[0], &CzeDoubleSpinBox::valueChanged, this, &OriginPropertyWidget::originXChanged);
	connect(originspinboxes[1], &CzeDoubleSpinBox::valueChanged, this, &OriginPropertyWidget::originYChanged);

	prop = propIn;
}

void OriginPropertyWidget::originPresetPressed(void* data)
{
	int i = *(int *)&data;
	prop->origin = QPointF((float) (i % 3) / 2, (float) (i / 3) / 2);
	updateSpinboxes();
}

void OriginPropertyWidget::originXChanged(double val)
{
	prop->origin.setX(val);
}

void OriginPropertyWidget::originYChanged(double val)
{
	prop->origin.setY(val);
}

void OriginPropertyWidget::updateSpinboxes()
{
	originspinboxes[0]->setValue(prop->origin.x());
	originspinboxes[1]->setValue(prop->origin.y());
}


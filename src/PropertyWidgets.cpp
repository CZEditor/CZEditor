
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
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
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
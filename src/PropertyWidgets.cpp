
#include "PropertyWidgets.hpp"
#include <qsizepolicy.h>
#include "Properties.hpp"

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
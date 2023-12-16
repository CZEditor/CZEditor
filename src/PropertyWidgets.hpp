#pragma once

#include <qspinbox.h>
#include <QWidget>

class IntProperty;
class CzeTextbox;

class IntPropertyWidget : public QWidget
{
public:
	IntPropertyWidget(IntProperty* propIn, QWidget* parent = nullptr);
	void textchanged(int);

	QSpinBox* textbox;
	IntProperty* prop;
};
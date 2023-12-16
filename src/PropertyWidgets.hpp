#pragma once

#include <qspinbox.h>
#include <QWidget>
#include "CzeSpinBox.hpp"

class IntProperty;
class CzeTextbox;

class IntPropertyWidget : public QWidget
{
public:
	IntPropertyWidget(IntProperty* propIn, QWidget* parent = nullptr);
	void textchanged(int);

	CzeSpinBox* textbox;
	IntProperty* prop;
};
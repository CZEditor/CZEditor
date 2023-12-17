#pragma once

#include <qspinbox.h>
#include <QWidget>
#include "CzeSpinBox.hpp"

class IntProperty;
class VertexProperty;
class CzeTextbox;

#define T(p) _Generic(p, IntProperty : "IntProperty", VertexProperty : "VertexProperty")

class IntPropertyWidget : public QWidget
{
public:
	IntPropertyWidget(IntProperty* propIn, QWidget* parent = nullptr);
	void textchanged(int);

	CzeSpinBox* textbox;
	IntProperty* prop;
};

class VertexPropertyWidget : public QWidget
{
public:
	VertexPropertyWidget(VertexProperty* propIn, QWidget* parent = nullptr);

	void textchanged(float value, int i);
	CzeSpinBox* values[3][3];
	VertexProperty* prop;
};
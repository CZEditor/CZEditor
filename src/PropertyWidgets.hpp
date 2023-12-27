#pragma once

#include <qspinbox.h>
#include <QWidget>
#include <QPainter>
#include "CzeSpinBox.hpp"
#include "CzeTextbox.hpp"

class IntProperty;
class VertexProperty;
class ColorProperty;
class StringProperty;
class SizeProperty;
class QuadProperty;

class CzeTextbox;

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
	CzeDoubleSpinBox* values[3][3];
	VertexProperty* prop;
};

class ColorPropertyWidget : public QWidget
{
public:
	ColorPropertyWidget(ColorProperty* propIn, QWidget* parent = nullptr);

	//void paintEvent(QPaintEvent* event);
	//void mousePressEvent(QMouseEvent* event);

	void colorSelected(QColor color);

	ColorProperty* prop;
};

class StringPropertyWidget : public QWidget
{
public:
	StringPropertyWidget(StringProperty* propIn, QWidget* parent = nullptr);
	void textchanged(QString);

	CzeTextbox* textbox;
	StringProperty* prop;
};


class QuadPropertyWidget : public QWidget
{
public:
	QuadPropertyWidget(QuadProperty* propIn, QWidget* parent = nullptr);

	void textchanged(float value, int i);
	CzeDoubleSpinBox* values[4][3];
	QuadProperty* prop;
};


class SizePropertyWidget : public QWidget 
{
public: 
	SizePropertyWidget(SizeProperty* propIn, QWidget* parent = nullptr); 
	void widthChanged(int);
	void heightChanged(int);
	SizeProperty* prop;
}
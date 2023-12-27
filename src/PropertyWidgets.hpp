#pragma once

#include <qspinbox.h>
#include <QWidget>
#include <QPainter>
#include "CzeSpinBox.hpp"
#include "CzeTextbox.hpp"
#include "CzeButton.hpp"
#include <QGridLayout>

class IntProperty;
class VertexProperty;
class ColorProperty;
class StringProperty;
class QuadProperty;
class OriginProperty;

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

class OriginPropertyWidget : public QWidget
{
private:
	static constexpr const char* buttonlabels[9] = {"\\", "^", "/", "<", "o", ">", "/", "v", "\\"};
public:
	OriginPropertyWidget(OriginProperty* propIn, QWidget* parent = nullptr);

	void originPresetPressed(void* data);
	void originXChanged(double val);
	void originYChanged(double val);

	void updateSpinboxes();

	QGridLayout* originpresetgridlayout;
	CzeButton* originpresetbuttons[9];
	CzeDoubleSpinBox* originspinboxes[2];

	OriginProperty* prop;
};


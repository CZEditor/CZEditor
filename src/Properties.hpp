#pragma once

#include "Data.hpp"
#include <random>
#include <float.h>
#include "PropertyWidgets.hpp"
#include "Property.hpp"
#include <qvector3d.h>
#include <QColor>
#include "Keyframes.hpp"

class IntProperty : public Property
{
public:
	IntProperty(IntData* dataIn):data(dataIn->data) { }
	virtual QString Serialize() { return data.Serialize(); }
	virtual void Deserialize(const QString serialized) { data.Deserialize(serialized); }
	virtual QWidget* Widget(QWidget* parent) { return new IntPropertyWidget(this,parent); }
	IntData data;
};

class VertexProperty : public Property
{
public:
	VertexProperty() { }
	virtual QString Serialize() { return "YEAH"; }
	virtual void Deserialize(const QString serialized) { return; }
	virtual QWidget* Widget(QWidget* parent) { return new VertexPropertyWidget(this, parent); }
	QVector3D vertices[3];
};

class ColorProperty : public Property
{
public:
	ColorProperty(QColor colorIn) : color(colorIn) { }
	virtual QString Serialize() { return ""; }
	virtual void Deserialize(const QString serialized) { return; }
	virtual QWidget* Widget(QWidget* parent) { return new ColorPropertyWidget(this, parent); }
	QColor color;
};

class StringProperty : public Property
{
public:
	StringProperty(QString textIn) : text(textIn) { }
	virtual QString Serialize() { return text; }
	virtual void Deserialize(const QString serialized) { return; }
	virtual QWidget* Widget(QWidget* parent) { return new StringPropertyWidget(this, parent); }
	QString text;
};

class QuadProperty : public Property
{
public:
	QuadProperty() { }
	virtual QString Serialize() { return "YEAH"; }
	virtual void Deserialize(const QString serialized) { return; }
	virtual QWidget* Widget(QWidget* parent) { return new QuadPropertyWidget(this, parent); }
	QVector3D vertices[4];
};

class SizeProperty : public Property
{
public:
	SizeProperty(int widthIn = 32, int heightIn = 32) : width(widthIn), height(heightIn) { }
	virtual QString Serialize() { return "YEAH"; }
	virtual void Deserialize(const QString serialized) { return; }
	virtual QWidget* Widget(QWidget* parent) { return new SizePropertyWidget(this, parent); }
	int width;
	int height;
};
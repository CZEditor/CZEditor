#pragma once

#include "Data.hpp"
#include <random>
#include <float.h>
#include "PropertyWidgets.hpp"
#include "Property.hpp"
#include <qvector3d.h>

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
	virtual QString Serialize() { return 0; }
	virtual void Deserialize(const QString serialized) { return; }
	virtual QWidget* Widget(QWidget* parent) { return new VertexPropertyWidget(this, parent); }
	QVector3D vertices[3];
};
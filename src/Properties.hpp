#pragma once

#include "Data.hpp"
#include <random>
#include <float.h>
#include "PropertyWidgets.hpp"
#include "Property.hpp"


class IntProperty : public Property
{
public:
	IntProperty(IntData* dataIn):data(dataIn->data) { }
	virtual QString Serialize() { return data.Serialize(); }
	virtual void Deserialize(const QString serialized) { data.Deserialize(serialized); }
	virtual QWidget* Widget(QWidget* parent) { return new IntPropertyWidget(this,parent); }
	IntData data;
};

class VerticeProperty : public Property
{
public:
	VerticeProperty() {}
	virtual QString Serialize() { return 0; }
	virtual void Deserialize(const QString serialized) { return; }
	virtual QWidget* Widget(QWidget* parent) { return new QWidget(parent); }
	void ChangeVertices(std::vector<float> &vertices)
	{
		vertices.push_back((float)(rand()) / (float)(RAND_MAX) * 5.0 - 2.5);
		vertices.push_back((float)(rand()) / (float)(RAND_MAX) * 5.0 - 2.5);
		vertices.push_back((float)(rand()) / (float)(RAND_MAX) * -5.0);
		vertices.push_back(0.0);
		vertices.push_back(0.0);
		vertices.push_back((float)(rand()) / (float)(RAND_MAX) * 5.0 - 2.5);
		vertices.push_back((float)(rand()) / (float)(RAND_MAX) * 5.0 - 2.5);
		vertices.push_back((float)(rand()) / (float)(RAND_MAX) * -5.0);
		vertices.push_back(0.0);
		vertices.push_back(1.0);
		vertices.push_back((float)(rand()) / (float)(RAND_MAX) * 5.0 - 2.5);
		vertices.push_back((float)(rand()) / (float)(RAND_MAX) * 5.0 - 2.5);
		vertices.push_back((float)(rand()) / (float)(RAND_MAX) * -5.0);
		vertices.push_back(1.0);
		vertices.push_back(0.0);
	}

};
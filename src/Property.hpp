#pragma once

#include "Serializable.hpp"
#include "Data.hpp"
#include <QWidget>
#include <random>
#include <float.h>

class Property : public Serializable
{
public:
	Property() {};
	virtual QString Serialize() = 0;
	virtual void Deserialize(const QString serialized) = 0;
	virtual QWidget* Widget() = 0;
};


class IntProperty : public Property
{
public:
	IntProperty(IntData* dataIn):data(dataIn->data) { }
	virtual QString Serialize() { return data.Serialize(); }
	virtual void Deserialize(const QString serialized) { data.Deserialize(serialized); }
	virtual QWidget* Widget() { return new QWidget(); }
	IntData data;
};

class VerticeProperty : public Property
{
public:
	VerticeProperty() {}
	virtual QString Serialize() { return 0; }
	virtual void Deserialize(const QString serialized) { return; }
	virtual QWidget* Widget() { return new QWidget(); }
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
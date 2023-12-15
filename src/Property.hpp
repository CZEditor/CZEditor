#pragma once

#include "Serializable.hpp"
#include "Data.hpp"
#include <QWidget>

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
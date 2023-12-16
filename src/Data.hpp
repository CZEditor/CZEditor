#pragma once
#include "Serializable.hpp"

class Data : public Serializable
{
public:
	Data() {};
	virtual QString Serialize() = 0;
	virtual void Deserialize(const QString serialized) = 0;
};

class IntData : public Data
{
public:
	IntData(int dataIn) { data = dataIn; }
	QString Serialize() { return QString::number(data); }
	void Deserialize(const QString serialized) { data = serialized.toLong(); }


	int data;
};
#pragma once
#include <QtCore/QString>

class Serializable
{
public:
	virtual QString Serialize() = 0;
	virtual void Deserialize(const QString serialized) = 0;
};
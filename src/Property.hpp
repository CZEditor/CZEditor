#pragma once

#include <QWidget>
#include "Serializable.hpp"

class Property : public Serializable
{
public:
	Property() {};
	virtual QString Serialize() = 0;
	virtual void Deserialize(const QString serialized) = 0;
	virtual QWidget* Widget(QWidget* parent) = 0;
	void* callbackData = 0;
	std::function<void(void*)> callback = 0;
};
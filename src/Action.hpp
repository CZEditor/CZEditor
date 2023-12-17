#pragma once

#include "Params.hpp"

class Action
{
public:
	virtual void actOnKeyframes() = 0;
	virtual Params* getDefaultParams() = 0;
};
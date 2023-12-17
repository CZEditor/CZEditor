#pragma once

#include "Params.hpp"

class Source
{
public:
	virtual void getImage() = 0;
	virtual Params* getDefaultParams() = 0;
};
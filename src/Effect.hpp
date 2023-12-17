#pragma once

#include "Params.hpp"

#define INIT_PARAMS(cls) cls() { params = getDefaultParams(); }

class Effect
{
public:
	virtual void effectVertices(std::vector<float>& vertices) = 0;
	virtual Params* getDefaultParams() = 0;

	Params* params;
};
#pragma once

#include "Params.hpp"



class Effect
{
public:
	virtual void effectVertices(std::vector<float>& vertices) = 0;
	virtual Params* getDefaultParams() = 0;

	Params* params;
};
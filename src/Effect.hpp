#pragma once

#include "Params.hpp"



class Effect : public KeyframeParam
{
public:
	virtual void effectVertices(std::vector<float>& vertices) = 0;
	
};
#pragma once

#include "Params.hpp"

#include "global.hpp"

class Effect : public KeyframeParam
{
public:
	virtual void effectVertices(std::vector<float>& verticesIn) = 0;
	virtual void addVertexShaders(std::list<VertexShader>& vertexshaders) = 0;
	virtual void addFragmentShaders(std::list<FragmentShader>& vertexshaders) = 0;
	
};
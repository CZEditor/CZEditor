#pragma once

#include "Params.hpp"

#include "OpenGLStructs.hpp"


class Effect : public KeyframeParam
{
public:
	virtual void effectVertices(std::vector<float>& verticesIn) = 0;
	virtual void addVertexShaders(std::list<VertexShader>& vertexshaders) = 0;
	virtual void addFragmentShaders(std::list<FragmentShader>& vertexshaders) = 0;
	virtual void initializeShaders(QOpenGLExtraFunctions extra) = 0;
};

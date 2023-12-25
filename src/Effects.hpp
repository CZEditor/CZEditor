#pragma once

#include "Effect.hpp"

#define RegisterEffect(name, className) RegisterKeyframeParam(name,className,EffectsDict)

extern KeyframeConstructorDict EffectsDict;

namespace Effects
{

	class SimpleVertexEffect : public Effect
	{
	public:
		INIT_PARAMS(SimpleVertexEffect)
		virtual void effectVertices(std::vector<float>& vertices);
		virtual void addVertexShaders(std::list<VertexShader>& vertexshaders) { return; };
		virtual void addFragmentShaders(std::list<FragmentShader>& vertexshaders) { return; };
		virtual void initializeShaders(QOpenGLExtraFunctions extra) { return; };
		virtual Params* getDefaultParams();

	};

	class SimpleImageEffect : public Effect
	{
	public:
		INIT_PARAMS(SimpleImageEffect)
		virtual void effectVertices(std::vector<float>& vertices) { return; };
		virtual void addVertexShaders(std::list<VertexShader>& vertexshaders) { return; };
		virtual void addFragmentShaders(std::list<FragmentShader>& fragmentshaders);
		virtual void initializeShaders(QOpenGLExtraFunctions extra);
		virtual Params* getDefaultParams();
		GLuint fragmentshader;
	};

};
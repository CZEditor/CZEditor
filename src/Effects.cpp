#include "Effects.hpp"
#include "Properties.hpp"


using namespace Effects;

KeyframeConstructorDict EffectsDict;

void SimpleVertexEffect::effectVertices(std::vector<float>& verticesIn)
{
	verticesIn.push_back(((VertexProperty*)params->elements["vertexfunc"])->vertices[0].x());
	verticesIn.push_back(((VertexProperty*)params->elements["vertexfunc"])->vertices[0].y());
	verticesIn.push_back(((VertexProperty*)params->elements["vertexfunc"])->vertices[0].z());
	verticesIn.push_back(0.0);
	verticesIn.push_back(0.0);
	verticesIn.push_back(((VertexProperty*)params->elements["vertexfunc"])->vertices[1].x());
	verticesIn.push_back(((VertexProperty*)params->elements["vertexfunc"])->vertices[1].y());
	verticesIn.push_back(((VertexProperty*)params->elements["vertexfunc"])->vertices[1].z());
	verticesIn.push_back(0.0);
	verticesIn.push_back(1.0);
	verticesIn.push_back(((VertexProperty*)params->elements["vertexfunc"])->vertices[2].x());
	verticesIn.push_back(((VertexProperty*)params->elements["vertexfunc"])->vertices[2].y());
	verticesIn.push_back(((VertexProperty*)params->elements["vertexfunc"])->vertices[2].z());
	verticesIn.push_back(1.0);
	verticesIn.push_back(0.0);
}


Params* SimpleVertexEffect::getDefaultParams()
{
	Params* ret = new Params();
	VertexProperty* v = new VertexProperty();
	v->vertices[0][0] = -1;
	v->vertices[0][1] = 0;
	v->vertices[0][2] = -3;
	v->vertices[1][0] = 0;
	v->vertices[1][1] = 1;
	v->vertices[1][2] = -3;
	v->vertices[2][0] = 1;
	v->vertices[2][1] = 0;
	v->vertices[2][2] = -3;
	ret->elements["vertexfunc"] = v;

	return ret;
}

RegisterEffect("Simple Vertex Effect", SimpleVertexEffect);

void SimpleImageEffect::addFragmentShaders(std::list<FragmentShader>& fragmentshaders)
{
	FragmentShader f;
	f.call = "SimpleImageEffect(pos, image);";
	f.declaration = "vec4 SimpleImageEffect(in vec2 pos, in sampler2D image);";
	f.type = FST_Color;
	f.shader = fragmentshader;
	fragmentshaders.push_back(f);
}

static const char* SimpleImageEffectFragment = "#version 460 core\n\
vec4 SimpleImageEffect(in vec2 pos, in sampler2D image)\n\
{\n\
return texture(image,pos);\n\
} \0";

void SimpleImageEffect::initializeShaders(QOpenGLExtraFunctions extra)
{
	
	fragmentshader = extra.glCreateShader(GL_FRAGMENT_SHADER);
	extra.glShaderSource(fragmentshader, 1, &SimpleImageEffectFragment,0);
	extra.glCompileShader(fragmentshader);
}

Params* SimpleImageEffect::getDefaultParams()
{
	Params* ret = new Params();

	return ret;
}

RegisterEffect("Simple Image Effect", SimpleImageEffect);
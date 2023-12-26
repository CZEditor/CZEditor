#include "Sources.hpp"
#include "Properties.hpp"
#include "Keyframes.hpp"
#include "global.hpp"
using namespace Sources;

KeyframeConstructorDict SourcesDict;

#define SetUpdateKeyframe(name) \
p->elements[name]->callback = [&](void* data) { UpdateKeyframeTexture((Keyframe*)data); };\
p->elements[name]->callbackData = keyframe

void ColorSource::getImage(unsigned char* img, int width, int height)
{
	int s = width * height * 4;
	QColor color = ((ColorProperty*)params->elements["color"])->color;
	for (int i = 0; i < s; i += 4)
	{
		img[i] = color.red();
		img[i+1] = color.green();
		img[i+2] = color.blue();
		img[i+3] = color.alpha();
	}
}

void ColorSource::getSize(int& width, int& height)
{
	width = ((IntProperty*)params->elements["width"])->data.data;
	height = ((IntProperty*)params->elements["height"])->data.data;
}

Params* ColorSource::getDefaultParams()
{
	Params* p = new Params();
	p->elements["width"] = new IntProperty(new IntData(32));
	p->elements["height"] = new IntProperty(new IntData(32));
	p->elements["color"] = new ColorProperty(QColor(127, 127, 127));
	p->elements["color"]->callback = [&](void* data) { UpdateKeyframeTexture((Keyframe*)data); };
	p->elements["color"]->callbackData = keyframe;
	return p;
}

RegisterSource("Color", ColorSource)



class ImageSource : public Source
{
public:
	INIT_PARAMS(ImageSource)
	virtual void getImage(unsigned char* img, int width, int height)
	{
		
	}
	virtual void getSize(int& width, int& height)
	{
		
	}
	virtual Params* getDefaultParams()
	{
		Params* p = new Params();
		p->elements["path"] = new StringProperty("");
		SetUpdateKeyframe("path");
		return p;
	}
};

RegisterSource("Image", ImageSource)
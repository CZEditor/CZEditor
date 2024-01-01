#include "Sources.hpp"
#include "Properties.hpp"
#include "Keyframes.hpp"
#include "global.hpp"
using namespace Sources;

KeyframeConstructorDict SourcesDict;

#define SetUpdateKeyframe(name) \
p->elements[name]->callback = &UpdateKeyframeTextureVoid;\
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
	width = ((SizeProperty*)params->elements["size"])->width;
	height = ((SizeProperty*)params->elements["size"])->height;
}

void UpdateKeyframeTextureVoid(void* data)
{
	UpdateKeyframeTexture((Keyframe*)data);
}

Params* ColorSource::getDefaultParams()
{
	Params* p = new Params();
	p->elements["size"] = new SizeProperty(32, 32);
	p->elements["size"]->callback = &UpdateKeyframeTextureVoid; p->elements["size"]->callbackData = keyframe;
	p->elements["color"] = new ColorProperty(QColor(127, 127, 127));
	p->elements["color"]->callback = &UpdateKeyframeTextureVoid; p->elements["color"]->callbackData = keyframe;
	return p;
}

RegisterSource("Color", ColorSource)


#include <qimage.h>

class ImageSource : public Source
{
public:
	INIT_PARAMS(ImageSource)
	virtual void getImage(unsigned char* imgIn, int width, int height)
	{
		memcpy(imgIn, img.bits(), img.sizeInBytes());
	}
	virtual void getSize(int& width, int& height)
	{
		img.load(((StringProperty*)(params->elements["path"]))->text);
		img.convertTo(QImage::Format_RGBA8888);
		width = img.width();
		height = img.height();
	}
	virtual Params* getDefaultParams()
	{
		Params* p = new Params();
		p->elements["path"] = new StringProperty("");
		p->elements["path"]->callback = &UpdateKeyframeTextureVoid; p->elements["path"]->callbackData = keyframe;
		return p;
	}
	QImage img;
};

RegisterSource("Image", ImageSource)

#include "Manager.hpp"
class TestSource : public Source
{
public:
	INIT_PARAMS(TestSource)
	virtual void getImage(unsigned char* imgIn, int width, int height)
	{
		uint8_t* yeah;
		epicmanager->getFrame(yeah);
		int second = -1;
		for (int i = -1; i < width * height * 4 - 1;)
		{
			imgIn[++i] = yeah[++second];
			imgIn[++i] = yeah[second];
			imgIn[++i] = yeah[second];
			imgIn[++i] = 255;
		}
		//memcpy(imgIn, yeah, width*height*3);
	}
	virtual void getSize(int& width, int& height)
	{
		epicmanager->getSize(width, height);
	}
	virtual Params* getDefaultParams()
	{
		Params* p = new Params();
		return p;
	}
	QImage img;
};

RegisterSource("Test", TestSource)
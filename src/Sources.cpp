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

#include "IVideoManager.hpp"
class TestSource : public Source
{
public:
	INIT_PARAMS(TestSource)
	virtual void getImage(unsigned char* imgIn, int width, int height)
	{
		if (vidhandle)
		{
			videomanager->getFrameRGBA(vidhandle, currentframe, imgIn);
		}
	}
	virtual void getSize(int& width, int& height)
	{
		if(vidhandle)
		{
			videomanager->getSize(vidhandle, width, height);
		}
	}
	virtual Params* getDefaultParams()
	{
		Params* p = new Params();
		p->elements["path"] = new StringProperty("C:\\Users\\relt\\Downloads\\v12044gd0000cl1fk0nog65tmrq2hda0.mov");
		return p;
	}
	virtual void checkForUpdate()
	{
		if (lastpath != ((StringProperty*)params->elements["path"])->text || vidhandle == 0)
		{
			lastpath = ((StringProperty*)params->elements["path"])->text;
			vidhandle = videomanager->openVideo(lastpath.toLocal8Bit().constData());
		}
		UpdateKeyframeTexture((Keyframe*)keyframe);
	}
	QString lastpath;
	AVHandle vidhandle = 0;
};

RegisterSource("Test", TestSource)
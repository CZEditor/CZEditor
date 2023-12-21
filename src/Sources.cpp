#include "Sources.hpp"
#include "Properties.hpp"
void ColorSource::getImage(unsigned char* img, int width, int height)
{
	int s = width * height * 4;
	unsigned char R = ((IntProperty*)params->elements["R"])->data.data;
	unsigned char G = ((IntProperty*)params->elements["G"])->data.data;
	unsigned char B = ((IntProperty*)params->elements["B"])->data.data;
	unsigned char A = ((IntProperty*)params->elements["A"])->data.data;
	for (int i = 0; i < s; i += 4)
	{
		img[i] = R;
		img[i+1] = G;
		img[i+2] = B;
		img[i+3] = A;
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
	p->elements["R"] = new IntProperty(new IntData(127));
	p->elements["G"] = new IntProperty(new IntData(255));
	p->elements["B"] = new IntProperty(new IntData(127));
	p->elements["A"] = new IntProperty(new IntData(255));
	return p;
}
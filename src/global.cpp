#include "global.hpp"
#include <time.h>
#include "Properties.hpp"

static int todayshue;
QFont defaultfont;
int currentframe = 0;
KeyframeList keyframelist;

ICZEditor* cze = 0;

void InitGlobals()
{
	todayshue = (time(nullptr)/(60*60*24) * 223) % 360;
	defaultfont = QFont("Segoe UI", 9);
}

QColor GetAccentColor(int saturation, int value, int alpha)
{
	QColor ret(0, 0, 0);
	ret.setHsv(todayshue, saturation, value, alpha);
	return ret;
}



#include "Keyframes.hpp"
void DoKeyframeShit(std::vector<float> &vertices)
{
	KeyframeList klist;
	for (auto& keyframe : keyframelist.keyframes)
	{
		for (auto& action : keyframe->actions)
		{
			action->actOnKeyframes(&klist, keyframe);
		}
	}
	for (auto& keyframe : klist.keyframes)
	{
		for (auto& effect : keyframe->effects)
		{
			effect->effectVertices(vertices);
		}
	}
}
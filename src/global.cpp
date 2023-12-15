#include "global.hpp"
#include <time.h>
#include "Keyframes.hpp"

static int todayshue;
QFont defaultfont;
int currentframe = 0;
KeyframeList keyframelist;

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

void DoKeyframeShit(std::vector<float> &vertices)
{
	for (auto& keyframe : keyframelist.keyframes)
	{
		for (auto& effect : keyframe->effects)
		{
			((VerticeProperty*)effect->elements["verticefunc"])->ChangeVertices(vertices);
		}
	}
}
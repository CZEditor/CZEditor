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

QColor InterpolateQColor(QColor colA, QColor colB, float i)
{
	QRgb B = colB.rgba();
	QRgb A = colA.rgba();
	int interpolation = i * 255;
	int interpolationM = 255 - interpolation;
	return QColor((qRed(B)  * interpolation + qRed(A) * interpolationM) / 255, (qGreen(B) * interpolation + qGreen(A) * interpolationM) / 255, (qBlue(B) * interpolation + qBlue(A) * interpolationM) / 255, (qAlpha(B) * interpolation + qAlpha(A) * interpolationM) / 255);
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
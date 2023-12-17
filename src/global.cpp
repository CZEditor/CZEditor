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
	if (keyframelist.keyframes.size() == 0)
		return;
	qWarning("%llx", keyframelist.keyframes[0]->effects[0]->elements.hash_function()("vertexfunc"));
	for (auto& keyframe : keyframelist.keyframes)
	{
		qWarning("%llx", keyframelist.keyframes[0]->effects[0]->elements.hash_function()("vertexfunc"));
		for (auto& effect : keyframe->effects)
		{
			qWarning("%llx", effect->elements.hash_function()("vertexfunc"));
			((VertexProperty*)(effect->elements["vertexfunc"]))->ChangeVertices(vertices);
		}
	}
}
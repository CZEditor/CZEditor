#include "global.hpp"
#include <time.h>

static int todayshue;

void InitGlobals()
{
	todayshue = (time(nullptr)/(60*60*24) * 17) % 360;
}

QColor GetAccentColor(int saturation, int value, int alpha)
{
	QColor ret(0, 0, 0);
	ret.setHsv(todayshue, saturation, value, alpha);
	return ret;
}
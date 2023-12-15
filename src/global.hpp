#pragma once

#include <qcolor.h>
#include <QFont>
#include "Keyframes.hpp"


extern void InitGlobals();
extern QColor GetAccentColor(int saturation, int value, int alpha = 255);

extern QFont defaultfont;

extern int currentframe;

extern KeyframeList keyframelist;

void DoKeyframeShit(std::vector<float> &vertices);
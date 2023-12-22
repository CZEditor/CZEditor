#pragma once

#include <qcolor.h>
#include <QFont>


extern void InitGlobals();
extern QColor GetAccentColor(int saturation, int value, int alpha = 255);
extern QColor InterpolateQColor(QColor colA, QColor colB, float i);

extern QFont defaultfont;

extern int currentframe;

#include "Keyframes.hpp"

extern KeyframeList keyframelist;

void DoKeyframeShit(std::vector<float> &vertices);

#include "ICZEditor.hpp"
extern ICZEditor* cze;
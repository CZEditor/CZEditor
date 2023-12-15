#pragma once

#include <qcolor.h>
#include <QFont>

extern void InitGlobals();
extern QColor GetAccentColor(int saturation, int value, int alpha = 255);

extern QFont defaultfont;
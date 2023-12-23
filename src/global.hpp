#pragma once

#include <qcolor.h>
#include <QFont>
#include <qopenglextrafunctions.h>
#include <qopenglfunctions.h>



extern void InitGlobals();
extern QColor GetAccentColor(int saturation, int value, int alpha = 255);
extern QColor InterpolateQColor(QColor colA, QColor colB, float i);

extern QFont defaultfont;

extern int currentframe;

#include "Keyframes.hpp"

extern KeyframeList keyframelist;
extern std::list<Keyframe*> uninitializedKeyframes;

void DoKeyframeShit(QOpenGLExtraFunctions extra, GLfloat* projectiondata);
void InitializeKeyframe(Keyframe* keyframe, QOpenGLExtraFunctions extra);

#include "ICZEditor.hpp"
extern ICZEditor* cze;


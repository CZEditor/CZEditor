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
#include <qopenglextrafunctions.h>
#include <qopenglfunctions.h>

void DoKeyframeShit(QOpenGLExtraFunctions extra);

#include "ICZEditor.hpp"
extern ICZEditor* cze;

struct VertexShader
{
	int shader;
	QString declaration;
	QString call;
};

enum FRAGMENT_SHADER_TYPE
{
	FST_Position,
	FST_Color
};

struct FragmentShader
{
	int shader;
	QString declaration;
	QString call;
	FRAGMENT_SHADER_TYPE type;
};
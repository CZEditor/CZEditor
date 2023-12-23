#pragma once

#include "Params.hpp"
#include <vector>
#include "Source.hpp"
#include "Action.hpp"
#include "Effect.hpp"
#include <qopenglfunctions.h>
class Keyframe
{
public:
	int frame;
	Source* source;
	std::list<Action*> actions;
	std::list<Effect*> effects;
	GLuint texture;
	std::list<GLuint> programs;
};

class KeyframeList
{
public:
	std::vector<Keyframe*> keyframes;
};
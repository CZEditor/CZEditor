#pragma once

#include "Params.hpp"
#include <vector>
#include "Source.hpp"
#include "Action.hpp"
#include "Effect.hpp"

class Keyframe
{
public:
	int frame;
	Source* source;
	std::list<Action*> actions;
	std::list<Effect*> effects;
	int texture;
};

class KeyframeList
{
public:
	std::vector<Keyframe*> keyframes;
};
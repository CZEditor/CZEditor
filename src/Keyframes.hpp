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
	std::vector<Action*> actions;
	std::vector<Effect*> effects;
};

class KeyframeList
{
public:
	std::vector<Keyframe*> keyframes;
};
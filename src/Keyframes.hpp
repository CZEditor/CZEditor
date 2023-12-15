#pragma once

#include "Params.hpp"
#include <vector>

class Keyframe
{
public:
	int frame;
	Params source;
	std::vector<Params*> actions;
	std::vector<Params*> effects;
};

class KeyframeList
{
public:
	std::vector<Keyframe*> keyframes;
};
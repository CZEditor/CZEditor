#pragma once

#include "Params.hpp"
#include <vector>

class Keyframe 
{
public:
	int frame;
	Params source;
	Params actions;
	Params effects;
};

class KeyframeList
{
public:
	std::vector<Keyframe*> keyframes;
};
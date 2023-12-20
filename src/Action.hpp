#pragma once

#include "Params.hpp"
//#include "Keyframes.hpp"

class KeyframeList;
class Keyframe;

class Action
{
public:
	virtual void actOnKeyframes(KeyframeList* klist, Keyframe* keyframe) = 0;
	virtual Params* getDefaultParams() = 0;

	Params* params;
};
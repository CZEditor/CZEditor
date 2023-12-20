#pragma once

#include "Params.hpp"
#include "Keyframes.hpp"

class Action
{
public:
	virtual void actOnKeyframes(KeyframeList* klist, Keyframe* keyframe) = 0;
	virtual Params* getDefaultParams() = 0;
};
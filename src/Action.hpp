#pragma once

#include "Params.hpp"
//#include "Keyframes.hpp"

class KeyframeList;
class Keyframe;

class Action : public KeyframeParam
{
public:
	virtual void actOnKeyframes(KeyframeList* klist, Keyframe* keyframe) = 0;
};
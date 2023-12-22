#pragma once
#include "Keyframes.hpp"

class ICZEditor
{
public:
	virtual void SelectKeyframe(Keyframe* keyframe) = 0;
};
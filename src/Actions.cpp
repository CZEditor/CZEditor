#include "Actions.hpp"

using namespace Actions;

void NormalKeyframe::actOnKeyframes(KeyframeList* klist, Keyframe* keyframe)
{
	klist->keyframes.push_back(keyframe);
}

Params* NormalKeyframe::getDefaultParams()
{
	Params* p = new Params();
	return p;
}
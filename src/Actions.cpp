#include "Actions.hpp"
#include "Keyframes.hpp"

using namespace Actions;

KeyframeConstructorDict ActionsDict;

void NormalKeyframe::actOnKeyframes(KeyframeList* klist, Keyframe* keyframe)
{
	klist->keyframes.push_back(keyframe);
}

Params* NormalKeyframe::getDefaultParams()
{
	Params* p = new Params();
	p->elements.clear();
	return p;
}

RegisterAction("Normal Keyframe", NormalKeyframe)
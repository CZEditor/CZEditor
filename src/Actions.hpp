#pragma once

#include "Action.hpp"

#define RegisterAction(name, className) RegisterKeyframeParam(name,className,ActionsDict)

extern KeyframeConstructorDict ActionsDict;

namespace Actions
{

	class NormalKeyframe : public Action
	{
	public:
		INIT_PARAMS(NormalKeyframe)
		virtual void actOnKeyframes(KeyframeList* klist, Keyframe* keyframe);
		virtual Params* getDefaultParams();
	};

}
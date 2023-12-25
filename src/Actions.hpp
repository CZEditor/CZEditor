#pragma once

#include "Action.hpp"

namespace Actions
{

	class NormalKeyframe : public Action
	{
	public:
		INIT_PARAMS(NormalKeyframe, "Normal Keyframe")
		virtual void actOnKeyframes(KeyframeList* klist, Keyframe* keyframe);
		virtual Params* getDefaultParams();
	};

}
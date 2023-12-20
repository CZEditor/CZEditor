#pragma once

#include "Action.hpp"

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
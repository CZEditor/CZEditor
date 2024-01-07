#pragma once
#include "AV.hpp"

class IAudioManager
{
public:
	virtual void getSize(AVHandle av, uint32_t& samples) = 0;
	virtual void getSamples(AVHandle av, uint32_t frameNumber, float* samples) = 0;
};

extern IAudioManager* audiomanager;
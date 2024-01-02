#pragma once
#include "qobject.h"

class Manager
{
public:
	virtual void getSize(int& widthOut, int& heightOut) = 0;
	virtual void getFrame(uint8_t* theframe) = 0;
	virtual void nextFrame() = 0;
};

extern Manager* epicmanager;
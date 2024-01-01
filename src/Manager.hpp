#pragma once

class Manager
{
public:
	virtual void getSize(int& widthOut, int& heightOut) = 0;
	virtual void getFrame(uint8_t* theframe) = 0;
};

extern Manager* epicmanager;
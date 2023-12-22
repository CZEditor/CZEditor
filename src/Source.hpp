#pragma once

#include "Params.hpp"

class Source : public KeyframeParam
{
public:
	virtual void getImage(unsigned char* img, int width, int height) = 0;
	virtual void getSize(int& width, int& height) = 0;
};
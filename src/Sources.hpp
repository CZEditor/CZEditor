#pragma once

#include "Source.hpp"

class ColorSource : public Source
{
public:
	INIT_PARAMS(ColorSource)
	virtual void getImage(unsigned char* img, int width, int height);
	virtual void getSize(int &width, int &height);
	virtual Params* getDefaultParams();
};
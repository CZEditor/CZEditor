#pragma once

typedef void* AVHandle;

class IVideoManager
{
public:
	virtual AVHandle openVideo(const char* path) = 0;
	virtual void getSize(AVHandle av,int& widthOut, int& heightOut) = 0;
	virtual void getFrameRGBA(AVHandle av, uint32_t frameNumber, uint8_t* data) = 0;
};

extern IVideoManager* videomanager;
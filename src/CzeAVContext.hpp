#pragma once

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
}

class CzeAVContext
{
public:
	AVCodecContext* video_codecctx = 0;
	AVCodecContext* audio_codecctx = 0;
	int video_stream = 0;
	int audio_stream = 0;
	AVFormatContext* formatctx = 0;
	SwsContext* swsctx = 0;
	SwrContext* swrctx = 0;

	AVFrame* video_frame = 0;
	AVFrame* audio_frame = 0;
	AVPacket* packet = 0;
};
#include <QString.h>
#include "Manager.hpp"

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}

class VideoManager : public Manager
{
public:
	VideoManager();
	void getSize(int& widthOut, int& heightOut)
	{
		widthOut = width;
		heightOut = height;
	}
	void getFrame(uint8_t* theframe)
	{
		int stride[] = {width*4 };
		sws_scale(swsctx, pFrame->data, pFrame->linesize, 0, height, &theframe, stride);
	}
	uint8_t* epic = 0;
	AVFrame* pFrame;
	int width = 0;
	int height = 0;
	SwsContext* swsctx;
};

static VideoManager themanger;
Manager* epicmanager = &themanger;

VideoManager::VideoManager()
{
	AVFormatContext* formatcontext = avformat_alloc_context();
	if (avformat_open_input(&formatcontext, "C:/Users/relt/Downloads/UUUAAAAAAAA.mp4.3gp", NULL, NULL) < 0)
	{
		qWarning("Error at %i", __LINE__);
		return;
	}
	if (avformat_find_stream_info(formatcontext, NULL) < 0)
	{
		qWarning("Error at %i", __LINE__);
		return;
	}
	int vid = -1;
	for (int i = 0; i < formatcontext->nb_streams; i++)
	{
		if (formatcontext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			vid = i;
			break;
		}
	}
	if (vid < 0)
	{
		qWarning("Error at %i", __LINE__);
		return;
	}
	const AVCodec* decoder = avcodec_find_decoder(formatcontext->streams[vid]->codecpar->codec_id);
	if (decoder == 0)
	{
		qWarning("Error at %i", __LINE__);
		return;
	}
	AVCodecContext* codecctx = avcodec_alloc_context3(decoder);
	if (avcodec_open2(codecctx, decoder, NULL) < 0)
	{
		qWarning("Error at %i", __LINE__);
		return;
	}
	pFrame = av_frame_alloc();
	if (!pFrame)
	{
		qWarning("Error at %i", __LINE__);
		return;
	}
	AVPacket* pPacket = av_packet_alloc();
	if (!pPacket)
	{
		qWarning("Error at %i", __LINE__);
		return;
	}
	while (true)
	{
		if (av_read_frame(formatcontext, pPacket) < 0)
		{
			qWarning("Error at %i", __LINE__);
			return;
		}
		if (pPacket->stream_index != vid)
		{
			qWarning("Error at %i", __LINE__);
			continue;
		}
		else
		{
			break;
		}
	}
	if (avcodec_send_packet(codecctx, pPacket) < 0)
	{
		qWarning("Error at %i", __LINE__);
		return;
	}
	if (avcodec_receive_frame(codecctx, pFrame) < 0)
	{
		qWarning("Error at %i", __LINE__);
		return;
	}
	
	epic = pFrame->data[0];
	width = codecctx->width;
	height = codecctx->height;
	swsctx = sws_getContext(width, height, codecctx->pix_fmt, width, height, AV_PIX_FMT_RGBA, SWS_BICUBIC, NULL, NULL, NULL);
	
	qWarning("%i %i %i", pFrame->width, pFrame->linesize[0], pFrame->linesize[0] * height);
}
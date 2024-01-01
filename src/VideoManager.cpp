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
	void nextFrame();
	uint8_t* epic = 0;
	AVFrame* pFrame;
	int width = 0;
	int height = 0;
	SwsContext* swsctx;
	AVPacket* pPacket2;
	AVCodecContext* codecctx;
	AVFormatContext* formatcontext2;
	AVFormatContext* formatcontext;
};

static VideoManager themanger;
Manager* epicmanager = &themanger;

AVFormatContext* OpenVid(const char* name, int& vid)
{
	AVFormatContext* formatcontext = avformat_alloc_context();
	if (avformat_open_input(&formatcontext, name, NULL, NULL) < 0)
	{
		qWarning("Error at %i", __LINE__);
		return 0;
	}

	if (avformat_find_stream_info(formatcontext, NULL) < 0)
	{
		qWarning("Error at %i", __LINE__);
		return 0;
	}
	vid = -1;
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
		return 0;
	}
	return formatcontext;
}

void GetNextFrame(AVFormatContext* formatcontext, AVFormatContext* formatcontext2, AVPacket* pPacket2, AVFrame* pFrame, AVCodecContext* codecctx)
{
	int err;
	while (true)
	{
		if ((err = av_read_frame(formatcontext2, pPacket2)) < 0)
		{
			av_seek_frame(formatcontext2, -1, 0, AVSEEK_FORCE);
			char why[64];
			av_make_error_string(why, 64, err);
			qWarning("Error at %i, %s", __LINE__, why);
			return;
		}
		//qWarning("%i", pPacket2->pos);
		if (formatcontext2->streams[pPacket2->stream_index]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			qWarning("%i", pPacket2->size);
			if (pPacket2->size < 300)
			{
				break;
			}
		}
	}
	for (int k = 0; k < 10; k++)
	{
		if (avcodec_send_packet(codecctx, pPacket2) < 0)
		{
			qWarning("Error at %i", __LINE__);
			return;
		}


		if (avcodec_receive_frame(codecctx, pFrame) < 0)
		{
			qWarning("Error at %i", __LINE__);
			return;
		}
	}
	/*while (true)
	{
		if (av_read_frame(formatcontext, pPacket2) < 0)
		{
			av_seek_frame(formatcontext, -1, 0, AVSEEK_FORCE);
			qWarning("Error at %i", __LINE__);
			return;
		}
		//qWarning("%i", pPacket2->pos);
		if (formatcontext->streams[pPacket2->stream_index]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			if (pPacket2->size < 300)
			{
				break;
			}
		}
	}
	for (int k = 0; k < 1; k++)
	{
		if (avcodec_send_packet(codecctx, pPacket2) < 0)
		{
			qWarning("Error at %i", __LINE__);
			return;
		}


		if (avcodec_receive_frame(codecctx, pFrame) < 0)
		{
			qWarning("Error at %i", __LINE__);
			return;
		}
	}
	*/
}

VideoManager::VideoManager()
{
	int vid = -1;
	formatcontext = OpenVid("C:\\Users\\relt\\Downloads\\feels_good_to_be_a_gangster_--_memes__fyp__viralMP4.mp4.3gp",vid);

	const AVCodec* decoder = avcodec_find_decoder(formatcontext->streams[vid]->codecpar->codec_id);
	if (decoder == 0)
	{
		qWarning("Error at %i", __LINE__);
		return;
	}
	codecctx = avcodec_alloc_context3(decoder);
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
	pPacket2 = av_packet_alloc();
	if (!pPacket2)
	{
		qWarning("Error at %i", __LINE__);
		return;
	}

	int vid2 = -1;
	//formatcontext2 = OpenVid("C:\\Users\\relt\\Downloads\\akj938-583.mov", vid2);
	formatcontext2 = formatcontext;

	//qWarning("%i", pPacket->pos);
	while (true)
	{
		if (av_read_frame(formatcontext, pPacket2) < 0)
		{
			qWarning("Error at %i", __LINE__);
			return;
		}
		if (formatcontext->streams[pPacket2->stream_index]->codecpar->codec_type == codecctx->codec_type)
		{
			break;
		}
	}
	if (avcodec_send_packet(codecctx, pPacket2) < 0)
	{
		qWarning("Error at %i", __LINE__);
		return;
	}


	if (avcodec_receive_frame(codecctx, pFrame) < 0)
	{
		qWarning("Error at %i", __LINE__);
		return;
	}
	GetNextFrame(formatcontext2, formatcontext, pPacket2, pFrame, codecctx);
		
	
	epic = pFrame->data[0];
	width = codecctx->width;
	height = codecctx->height;
	swsctx = sws_getContext(width, height, codecctx->pix_fmt, width, height, AV_PIX_FMT_RGBA, SWS_BICUBIC, NULL, NULL, NULL);
	
	qWarning("%i %i %i", pFrame->width, pFrame->linesize[0], pFrame->linesize[0] * height);
}

void VideoManager::nextFrame()
{
	GetNextFrame(formatcontext2, formatcontext, pPacket2, pFrame, codecctx);
}
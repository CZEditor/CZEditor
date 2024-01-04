#include <QAudioSink>
#include <QtMultimedia/qaudiodevice.h>
#include <QtMultimedia/qaudiooutput.h>
#include <QMediaDevices>
#include "IVideoManager.hpp"
#include <qbuffer.h>
#include "CzeAVContext.hpp"

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}

class VideoManager : public IVideoManager
{
public:
	//VideoManager();
	virtual AVHandle openVideo(const char* path);
	virtual void getSize(AVHandle av, int& widthOut, int& heightOut);
	virtual void getFrameRGBA(AVHandle av, uint32_t frameNumber, uint8_t* data);
private:
	void seekToFrame(CzeAVContext* av, uint32_t frameNumber);
	/*void getSize(int& widthOut, int& heightOut)
	{
		widthOut = width;
		heightOut = height;
	}
	void getFrame(uint8_t* theframe)
	{
		int stride[] = {width*4 };
		sws_scale(swsctx, pFrame->data, pFrame->linesize, 0, height, &theframe, stride);
	}
	*/
	//void keepPlaying(QAudio::State thestate)
	//{
		//qWarning("pos: %i", buf->pos());
		//if (thestate == QAudio::IdleState)
		//{
		//buf->seek(0);
		//}
		//qWarning("%i",thestate);
	//}
	//void nextFrame();
	//AVFrame* pFrame;
	//AVFrame* pFrameAudio;
	//SwsContext* swsctx;
	//AVPacket* pPacket2;
	//QIODevice* buf;
	//QAudioSink* player;
};


static VideoManager s_videomanger;
IVideoManager* videomanager = &s_videomanger;




AVHandle VideoManager::openVideo(const char* path)
{
	CzeAVContext* av = new CzeAVContext();
	av->formatctx = avformat_alloc_context();
	if (avformat_open_input(&av->formatctx, path, NULL, NULL) < 0)
	{
		qWarning("Error at %i", __LINE__);
		return 0;
	}

	if (avformat_find_stream_info(av->formatctx, NULL) < 0)
	{
		qWarning("Error at %i", __LINE__);
		return 0;
	}


	av->video_stream = -1;
	av->audio_stream = -1;
	for (int i = 0; i < av->formatctx->nb_streams; i++)
	{
		if (av->formatctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			av->video_stream = i;
		}
		if (av->formatctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			av->audio_stream = i;
		}
	}

	const AVCodec* video_decoder = avcodec_find_decoder(av->formatctx->streams[av->video_stream]->codecpar->codec_id);
	const AVCodec* audio_decoder = avcodec_find_decoder(av->formatctx->streams[av->audio_stream]->codecpar->codec_id);
	if (video_decoder == 0 || audio_decoder == 0)
	{
		qWarning("Error at %i", __LINE__);
		return 0;
	}


	av->video_codecctx = avcodec_alloc_context3(video_decoder);
	av->video_codecctx->err_recognition = AV_EF_IGNORE_ERR;
	av->video_codecctx->strict_std_compliance = FF_COMPLIANCE_VERY_STRICT;

	avcodec_parameters_to_context(av->video_codecctx, av->formatctx->streams[av->video_stream]->codecpar);

	if (avcodec_open2(av->video_codecctx, video_decoder, NULL) < 0)
	{
		qWarning("Error at %i", __LINE__);
		return 0;
	}


	av->audio_codecctx = avcodec_alloc_context3(audio_decoder);
	av->audio_codecctx->err_recognition = AV_EF_IGNORE_ERR;
	av->audio_codecctx->strict_std_compliance = FF_COMPLIANCE_VERY_STRICT;

	avcodec_parameters_to_context(av->audio_codecctx, av->formatctx->streams[av->audio_stream]->codecpar);

	if (avcodec_open2(av->audio_codecctx, audio_decoder, NULL) < 0)
	{
		qWarning("Error at %i", __LINE__);
		return 0;
	}
	av->video_frame = av_frame_alloc();
	av->audio_frame = av_frame_alloc();
	av->packet = av_packet_alloc();
	av->swsctx = sws_getContext(
		av->video_codecctx->width, 
		av->video_codecctx->height, 
		av->video_codecctx->pix_fmt, 
		av->video_codecctx->width, 
		av->video_codecctx->height,
		AV_PIX_FMT_RGBA, SWS_POINT, NULL, NULL, NULL);
	return (AVHandle)av;
}

void VideoManager::getSize(AVHandle avhandle, int& widthOut, int& heightOut)
{
	CzeAVContext* av = (CzeAVContext*)avhandle;
	widthOut = av->video_codecctx->width;
	heightOut = av->video_codecctx->height;
}

void VideoManager::getFrameRGBA(AVHandle avhandle, uint32_t frameNumber, uint8_t* data)
{
	CzeAVContext* av = (CzeAVContext*)avhandle;
	AVRational currentframe = av_mul_q(av->formatctx->streams[av->video_stream]->r_frame_rate, av_mul_q(av->formatctx->streams[av->video_stream]->time_base, av_make_q(av->video_frame->pts, 1)));
	int tbase = av->formatctx->streams[av->video_stream]->time_base.den / av->formatctx->streams[av->video_stream]->time_base.num;
	if (currentframe.num > frameNumber)
	{
		
		qWarning("%i/%i != %i", currentframe.num, currentframe.den, frameNumber);
		
		// Not the next frame, have to seek
		seekToFrame(av, frameNumber* tbase);
	}
	int err = 0;

	while (true)
	{
		err = av_read_frame(av->formatctx, av->packet); // Get packet from file
		if (err < 0)
		{
			if (err == AVERROR_EOF)
			{
				//seekToFrame(av, 0); // Loop
				//err = av_read_frame(av->formatctx, av->packet);
				break;
			}
			// TODO : Add more error handling later
		}
		if (av->formatctx->streams[av->packet->stream_index]->codecpar->codec_type != AVMEDIA_TYPE_VIDEO)
		{
			continue;
		}
		err = avcodec_send_packet(av->video_codecctx, av->packet);        // Send packet
		if (err < 0)
		{
			qWarning("%i: %x", __LINE__,err);
		}
		err = avcodec_receive_frame(av->video_codecctx, av->video_frame); // get frame
		if (err < 0)
		{
			if (err == AVERROR(EAGAIN))
			{
				continue;
			}
		}
		currentframe = av_mul_q(av->formatctx->streams[av->video_stream]->r_frame_rate, av_mul_q(av->formatctx->streams[av->video_stream]->time_base, av_make_q(av->video_frame->pts, 1)));
		if (currentframe.num < frameNumber)
		{
			
			continue;
		}
		break;
	}
	//qWarning("%i/%i | %i", currentframe.num, currentframe.den, frameNumber);
	
	

	int stride = av->video_frame->width * 4; // 4 bytes per pixel

	sws_scale(av->swsctx, av->video_frame->data, av->video_frame->linesize, 0, av->video_frame->height, &data, &stride); // Convert YUV to RGBA

}

void VideoManager::seekToFrame(CzeAVContext* av, uint32_t frameNumber)
{
	av_seek_frame(av->formatctx, av->video_stream, frameNumber, AVSEEK_FLAG_ANY|AVSEEK_FORCE);
}

/*
AVFormatContext* OpenVid(const char* name, int& vid, int& audio)
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
		}
		if (formatcontext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			audio = i;
		}
	}
	return formatcontext;
}

*/
void GetNextFrame(AVFormatContext* formatcontext, AVFormatContext* formatcontext2, AVPacket* pPacket2, AVFrame* pFrame, AVFrame* pFrameAudio, AVCodecContext* codecctx, AVCodecContext* audiocodecctx, QIODevice* buf, QAudioSink* player, bool notinit)
{
	int err;
	char why[64];
	

	bool audio = false;
	bool video = false;
	for(int d = 0; d < 2; d++)
	{
		if ((err = av_read_frame(formatcontext2, pPacket2)) < 0)
		{
			av_seek_frame(formatcontext2, -1, 0, AVSEEK_FORCE);
			av_make_error_string(why, 64, err);
			qWarning("Error at %i, %s", __LINE__, why);
			return;
		}
		//pPacket2->data[rand()*pPacket2->size/RAND_MAX]++;
		//pPacket2->data[pPacket2->size / 4] = 0;
		if (formatcontext2->streams[pPacket2->stream_index]->codecpar->codec_type == codecctx->codec_type && !video)
		//if(pPacket2->stream_index == 0)
		{
			for (int k = 0; k < 1; k++)
			{
				if (avcodec_send_packet(codecctx, pPacket2) < 0)
				{
					qWarning("Error at %i", __LINE__);
					//return;
				}


				if ((err = avcodec_receive_frame(codecctx, pFrame)) < 0)
				{
					if (err == AVERROR(EAGAIN))
					{
						break;
					}
					av_make_error_string(why, 64, err);
					qWarning("Error at %i, %s", __LINE__, why);
					//return;
				}
			}
			//if (err == AVERROR(EAGAIN))
			//{
			//	continue;
			//}
			video = true;
		}
		else if (formatcontext2->streams[pPacket2->stream_index]->codecpar->codec_type == audiocodecctx->codec_type && !audio)
		{
			if (avcodec_send_packet(audiocodecctx, pPacket2) < 0)
			{
				qWarning("Error at %i", __LINE__);
				//return;
			}


			if (avcodec_receive_frame(audiocodecctx, pFrameAudio) < 0)
			{
				qWarning("Error at %i", __LINE__);
				//return;
			}
			if (buf)
			{
				buf->write((char*)(pFrameAudio->extended_data[0]), pFrameAudio->linesize[0]);
			}
			audio = true;
			//qWarning("%x", pFrameAudio->extended_data[0]);
			
			
		}
		if (video && audio)
		{
			break;
		}
	}
	/*

	while (notinit)
	{
		if ((err = av_read_frame(formatcontext, pPacket2)) < 0)
		{
			av_seek_frame(formatcontext, -1, 0, AVSEEK_FORCE);
			av_make_error_string(why, 64, err);
			qWarning("Error at %i, %s", __LINE__, why);
			return;
		}
		if (formatcontext->streams[pPacket2->stream_index]->codecpar->codec_type == codecctx->codec_type)
		{
			for (int k = 0; k < 1; k++)
			{
				if (avcodec_send_packet(codecctx, pPacket2) < 0)
				{
					qWarning("Error at %i", __LINE__);
					return;
				}


				if ((err = avcodec_receive_frame(codecctx, pFrame)) < 0)
				{
					if (err == AVERROR(EAGAIN))
					{
						break;
					}
					av_make_error_string(why, 64, err);
					qWarning("Error at %i, %s", __LINE__, why);
					return;
				}
			}
			//if (err == AVERROR(EAGAIN))
			//{
			//	continue;
			//}
			break;
		}
	}
	*/
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
/*
const char justnothing[2048] = { };

VideoManager::VideoManager()
{
	//QAudioDevice adevice = QAudioDevice(QMediaDevices::defaultAudioOutput());
	QAudioOutput* output = new QAudioOutput;
	QAudioFormat format;
	
	//connect(player, &QAudioSink::stateChanged, this, &VideoManager::keepPlaying);
	
	//player->start(buf);
	
	av_log_set_level(AV_LOG_FATAL);
	

	int vid = -1;
	int audio = -1;
	formatcontext = OpenVid("C:\\Users\\micha\\Downloads\\4k)_720P HD.mp4",vid,audio);

	const AVCodec* decoder = avcodec_find_decoder(formatcontext->streams[vid]->codecpar->codec_id);
	const AVCodec* audiodecoder = avcodec_find_decoder(formatcontext->streams[audio]->codecpar->codec_id);
	if (decoder == 0 || audiodecoder == 0)
	{
		qWarning("Error at %i", __LINE__);
		return;
	}
	codecctx = avcodec_alloc_context3(decoder);
	codecctx->err_recognition = AV_EF_IGNORE_ERR;
	codecctx->strict_std_compliance = FF_COMPLIANCE_VERY_STRICT;
	avcodec_parameters_to_context(codecctx, formatcontext->streams[vid]->codecpar);
	if (avcodec_open2(codecctx, decoder, NULL) < 0)
	{
		qWarning("Error at %i", __LINE__);
		return;
	}
	audiocodecctx = avcodec_alloc_context3(audiodecoder);
	audiocodecctx->err_recognition = AV_EF_IGNORE_ERR;
	audiocodecctx->strict_std_compliance = FF_COMPLIANCE_VERY_STRICT;
	avcodec_parameters_to_context(audiocodecctx, formatcontext->streams[audio]->codecpar);
	if (avcodec_open2(audiocodecctx, audiodecoder, NULL) < 0)
	{
		qWarning("Error at %i", __LINE__);
		return;
	}


	pFrame = av_frame_alloc();
	pFrameAudio = av_frame_alloc();
	if (!pFrame || !pFrameAudio)
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
	formatcontext2 = OpenVid("C:\\Users\\relt\\Downloads\\Spongebob - Dont Need It! [original].mp4", vid2, audio);
	//formatcontext2 = formatcontext;

	//qWarning("%i", pPacket->pos);
	if (av_read_frame(formatcontext, pPacket2) < 0)
	{
		qWarning("Error at %i", __LINE__);
		return;
	}
	if (formatcontext->streams[pPacket2->stream_index]->codecpar->codec_type == codecctx->codec_type)
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
		
	GetNextFrame(formatcontext2, formatcontext, pPacket2, pFrame, pFrameAudio,codecctx, audiocodecctx, buf, player,false);
	
	width = codecctx->width;
	height = codecctx->height;
	swsctx = sws_getContext(width, height, codecctx->pix_fmt, width, height, AV_PIX_FMT_RGBA, SWS_POINT, NULL, NULL, NULL);
	
	format.setSampleRate(audiocodecctx->sample_rate);
	format.setChannelCount(1);
	format.setSampleFormat(QAudioFormat::Float);

	player = new QAudioSink(format);
	player->setBufferSize(pFrameAudio->linesize[0]);
	buf = player->start();


	qWarning("%i %i %i", pFrame->width, pFrame->linesize[0], pFrame->linesize[0] * height);
}

void VideoManager::nextFrame()
{
	GetNextFrame(formatcontext2, formatcontext, pPacket2, pFrame, pFrameAudio, codecctx, audiocodecctx, buf, player,true);
}
*/

#include <QAudioSink>
#include <QtMultimedia/qaudiodevice.h>
#include <QtMultimedia/qaudiooutput.h>
#include <QMediaDevices>
#include "Manager.hpp"
#include <qbuffer.h>

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}

class VideoManager : public QObject, public Manager
{
	Q_OBJECT
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
	void keepPlaying(QAudio::State thestate)
	{
		qWarning("pos: %i", buf->pos());
		//if (thestate == QAudio::IdleState)
		//{
		//buf->seek(0);
		//}
		//qWarning("%i",thestate);
	}
	void nextFrame();
	uint8_t* epic = 0;
	AVFrame* pFrame;
	AVFrame* pFrameAudio;
	int width = 0;
	int height = 0;
	SwsContext* swsctx;
	AVPacket* pPacket2;
	AVCodecContext* codecctx;
	AVCodecContext* audiocodecctx;
	AVFormatContext* formatcontext2;
	AVFormatContext* formatcontext;
	QIODevice* buf;

	QAudioSink* player;
};

static VideoManager themanger;
Manager* epicmanager = &themanger;

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
		pPacket2->data[rand()*pPacket2->size/RAND_MAX]++;
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
	/*if (av_read_frame(formatcontext, pPacket2) < 0)
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
	}*/
		
	GetNextFrame(formatcontext2, formatcontext, pPacket2, pFrame, pFrameAudio,codecctx, audiocodecctx, buf, player,false);
	
	width = codecctx->width;
	height = codecctx->height;
	swsctx = sws_getContext(width, height, codecctx->pix_fmt, width, height, AV_PIX_FMT_RGBA, SWS_BICUBIC, NULL, NULL, NULL);
	
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

#include "VideoManager.moc"
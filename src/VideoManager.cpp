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

class Generator : public QIODevice
{
	Q_OBJECT

public:
	Generator();

	void start();
	void stop();

	qint64 readData(char* data, qint64 maxlen) override;
	qint64 writeData(const char* data, qint64 len) override;
	qint64 bytesAvailable() const override;
	qint64 size() const override { return thesize; }
	char* dat;
	int thesize = 0;
};

Generator::Generator()
{
}

void Generator::start()
{
	open(QIODevice::ReadOnly);
}

void Generator::stop()
{
	close();
}


qint64 Generator::readData(char* data, qint64 len)
{
	qint64 total = 0;
	memcpy(data, dat, thesize);
	return total;
}

qint64 Generator::writeData(const char* data, qint64 len)
{
	Q_UNUSED(data);
	Q_UNUSED(len);

	return 0;
}

qint64 Generator::bytesAvailable() const
{
	return thesize;
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

void GetNextFrame(AVFormatContext* formatcontext, AVFormatContext* formatcontext2, AVPacket* pPacket2, AVFrame* pFrame, AVFrame* pFrameAudio, AVCodecContext* codecctx, AVCodecContext* audiocodecctx, QIODevice* buf, QAudioSink* player)
{
	int err;
	char why[64];
	while (true)
	{
		if ((err = av_read_frame(formatcontext2, pPacket2)) < 0)
		{
			av_seek_frame(formatcontext2, -1, 0, AVSEEK_FORCE);
			av_make_error_string(why, 64, err);
			qWarning("Error at %i, %s", __LINE__, why);
			return;
		}
		//if (formatcontext2->streams[pPacket2->stream_index]->codecpar->codec_type == codecctx->codec_type)
		if(pPacket2->stream_index == 0)
		{
			for (int k = 0; k < 3; k++)
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
			if (err == AVERROR(EAGAIN))
			{
				continue;
			}
			break;
		}
		else if (pPacket2->stream_index == 1)
		{
			if (avcodec_send_packet(audiocodecctx, pPacket2) < 0)
			{
				qWarning("Error at %i", __LINE__);
				return;
			}


			if (avcodec_receive_frame(audiocodecctx, pFrameAudio) < 0)
			{
				qWarning("Error at %i", __LINE__);
				return;
			}
			buf->write((char*)(pFrameAudio->extended_data[0]), pFrameAudio->linesize[0]);
			qWarning("%x", pFrameAudio->extended_data[0]);
			

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

const char justnothing[2048] = { };

VideoManager::VideoManager()
{
	//QAudioDevice adevice = QAudioDevice(QMediaDevices::defaultAudioOutput());
	QAudioOutput* output = new QAudioOutput;
	QAudioFormat format;
	format.setSampleRate(44100);
	format.setChannelCount(1);
	format.setSampleFormat(QAudioFormat::Float);
	player = new QAudioSink(format);
	//connect(player, &QAudioSink::stateChanged, this, &VideoManager::keepPlaying);
	
	//player->start(buf);
	player->setBufferSize(8192);
	buf = player->start();
	

	int vid = -1;
	int audio = -1;
	formatcontext = OpenVid("C:\\Users\\relt\\Downloads\\EED2FF76-84E6-41EF-A028-A48320F496E2.mov",vid,audio);

	const AVCodec* decoder = avcodec_find_decoder(formatcontext->streams[vid]->codecpar->codec_id);
	const AVCodec* audiodecoder = avcodec_find_decoder(formatcontext->streams[audio]->codecpar->codec_id);
	if (decoder == 0 || audiodecoder == 0)
	{
		qWarning("Error at %i", __LINE__);
		return;
	}
	codecctx = avcodec_alloc_context3(decoder);
	avcodec_parameters_to_context(codecctx, formatcontext->streams[vid]->codecpar);
	if (avcodec_open2(codecctx, decoder, NULL) < 0)
	{
		qWarning("Error at %i", __LINE__);
		return;
	}
	audiocodecctx = avcodec_alloc_context3(audiodecoder);
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
	//formatcontext2 = OpenVid("C:\\Users\\relt\\Downloads\\akj938-583.mov", vid2);
	formatcontext2 = formatcontext;

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
	GetNextFrame(formatcontext2, formatcontext, pPacket2, pFrame, pFrameAudio,codecctx, audiocodecctx, buf, player);
		
	
	epic = pFrame->data[0];
	width = codecctx->width;
	height = codecctx->height;
	swsctx = sws_getContext(width, height, codecctx->pix_fmt, width, height, AV_PIX_FMT_RGBA, SWS_BICUBIC, NULL, NULL, NULL);
	
	qWarning("%i %i %i", pFrame->width, pFrame->linesize[0], pFrame->linesize[0] * height);
}

void VideoManager::nextFrame()
{
	GetNextFrame(formatcontext2, formatcontext, pPacket2, pFrame, pFrameAudio, codecctx, audiocodecctx, buf, player);
}

#include "VideoManager.moc"
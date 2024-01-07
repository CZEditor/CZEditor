#include <QAudioSink>
#include <QtMultimedia/qaudiodevice.h>
#include <QtMultimedia/qaudiooutput.h>
#include <QMediaDevices>
#include "IAudioManager.hpp"
#include <qbuffer.h>
#include "CzeAVContext.hpp"

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
}

class AudioManager : public IAudioManager
{
public:
	virtual void getSize(AVHandle av, uint32_t& samples);
	virtual void getSamples(AVHandle av, uint32_t frameNumber, float* samples);
private:
	void seekToFrame(CzeAVContext* av, uint32_t frameNumber);
};


static AudioManager s_audiomanager;
IAudioManager* audiomanager = &s_audiomanager;


void AudioManager::getSize(AVHandle avhandle, uint32_t& samples)
{
	CzeAVContext* av = (CzeAVContext*)avhandle;
	samples = av->audio_frame->nb_samples;
}

void AudioManager::getSamples(AVHandle avhandle, uint32_t frameNumber, float* data)
{
	CzeAVContext* av = (CzeAVContext*)avhandle;
	AVRational currentframe = av_mul_q(av->formatctx->streams[av->audio_stream]->r_frame_rate, av_mul_q(av->formatctx->streams[av->audio_stream]->time_base, av_make_q(av->audio_frame->pts, 1)));
	int tbase = av->formatctx->streams[av->audio_stream]->time_base.den / av->formatctx->streams[av->audio_stream]->time_base.num;
	if (currentframe.num > frameNumber)
	{

		qWarning("%i/%i != %i", currentframe.num, currentframe.den, frameNumber);

		// Not the next frame, have to seek
		seekToFrame(av, frameNumber * tbase);
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
		if (av->formatctx->streams[av->packet->stream_index]->codecpar->codec_type != AVMEDIA_TYPE_AUDIO)
		{
			continue;
		}
		err = avcodec_send_packet(av->audio_codecctx, av->packet);        // Send packet
		if (err < 0)
		{
			qWarning("%i: %x", __LINE__, err);
		}
		err = avcodec_receive_frame(av->audio_codecctx, av->audio_frame); // get frame
		if (err < 0)
		{
			if (err == AVERROR(EAGAIN))
			{
				continue;
			}
		}
		currentframe = av_mul_q(av->formatctx->streams[av->audio_stream]->r_frame_rate, av_mul_q(av->formatctx->streams[av->audio_stream]->time_base, av_make_q(av->audio_frame->pts, 1)));
		if (currentframe.num < frameNumber)
		{
			continue;
		}
		break;
	}
}

void AudioManager::seekToFrame(CzeAVContext* av, uint32_t frameNumber)
{
	av_seek_frame(av->formatctx, av->audio_stream, frameNumber, AVSEEK_FLAG_ANY | AVSEEK_FORCE);
}

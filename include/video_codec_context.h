#pragma once
#ifndef _VIDEO_CODEC_CONTENT_
#define _VIDEO_CODEC_CONTENT_

#define INBUF_SIZE 4096
#define AUDIO_INBUF_SIZE 20480
#define AUDIO_REFILL_THRESH 4096

extern "C"
{
    #include <libavutil/opt.h>
    #include <libavutil/common.h>
    #include <libavutil/imgutils.h>
    #include <libavutil/samplefmt.h>
    #include <libavutil/mathematics.h>
    #include <libavutil/channel_layout.h>
    #include <libavcodec/avcodec.h>
}

typedef struct
{
    AVCodec                 *pCodec;
    AVCodecContext          *pCodecContext;
    AVCodecParserContext    *pCodecParserCtx;
    AVFrame                 *frame;
    AVPacket                pkt;
} CodecCtx;

#endif
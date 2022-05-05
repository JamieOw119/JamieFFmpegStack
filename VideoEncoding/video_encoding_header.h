#pragma once
#ifndef _VIDEO_ENCODING_HEADER_
#define _VIDEO_ENCODING_HEADER_

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
    AVCodec     *codec;
    AVFrame     *frame;
    AVCodecContext  *c;
    AVPacket    pkt;
} CodecCtx;

#endif
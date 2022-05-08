#pragma once
#ifndef _CODEC_CTX_H_
#define _CODEC_CTX_H_

extern "C"
{
#include <libavcodec/avcodec.h>
}

typedef struct
{
    const AVCodec           *codec;
    AVCodecParserContext    *parser;
    AVCodecContext          *ctx;
    AVFrame                 *frame;
    AVPacket                *pkt;
} CodecCtx;

void InitCodecCtx(CodecCtx &codec_ctx, enum AVCodecID);

void CloseCodecCtx(CodecCtx &codec_ctx);

#endif
#pragma once
#ifndef _CODEC_CTX_H_
#define _CODEC_CTX_H_

#include "input_output.h"

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
}

typedef struct
{
    const AVCodec           *codec;
    AVCodecParserContext    *parser;
    AVCodecContext          *ctx;
    AVFrame                 *frame;
    AVPacket                *pkt;
} CodecCtx;

void InitDecodeCodecCtx(CodecCtx &codec_ctx, enum AVCodecID);

void InitEncodeCodecCtx(CodecCtx &codec_ctx, IOParam io_param, enum AVCodecID avcodec_id);

void SetEncodeCodecCtx(CodecCtx &codec_ctx, IOParam io_param);

void CloseCodecCtx(CodecCtx &codec_ctx, bool isDecoding);

#endif
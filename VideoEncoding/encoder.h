#pragma once
#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "codec_ctx.h"
#include "input_output.h"

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
}

void Encode(CodecCtx &codec_ctx, IOParam &io_param);

void EncodeCore(CodecCtx &codec_ctx, IOParam &io_param);

void ReadInYUVData(CodecCtx &codec_ctx, IOParam &io_param);

#endif
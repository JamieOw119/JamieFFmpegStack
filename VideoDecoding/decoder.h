#pragma once
#ifndef _DECODER_H_
#define _DECODER_H_

#include "codec_ctx.h"
#include "input_output.h"

extern "C"
{
#include <libavcodec/avcodec.h>
}

#define DECODE_BUF_SIZE 1024

void Decode(CodecCtx &codec_ctx, IOParam &io_param);

void DecodeCore(CodecCtx &codec_ctx, IOParam &io_param);

void WriteOutYUVFile(CodecCtx &codec_ctx, IOParam &io_param);

#endif
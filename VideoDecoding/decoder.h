#pragma once
#ifndef _DECODER_H_
#define _DECODER_H_
#include "video_codec_context.h"

bool Open_decoder(CodecCtx &ctx);

void Close_decoder(CodecCtx &ctx);

#endif
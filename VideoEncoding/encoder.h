#pragma once
#ifndef _ENCODER_H_
#define _ENCODER_H_
#include "video_encoding_header.h"
#include "../include/input_output.h"

bool Open_encoder(CodecCtx &ctx, IOParam io_param);

void Close_encoder(CodecCtx &ctx);

#endif
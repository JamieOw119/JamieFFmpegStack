#pragma once
#ifndef _INPUT_OUTPUT_H_
#define _INPUT_OUTPUT_H_

#include <stdio.h>
#include <stdint.h>
#include "video_codec_context.h"

typedef struct 
{
    FILE        *pFin;
    FILE        *pFout;

    char        *pNameIn;
    char        *pNameOut;

    uint16_t    nImageWidth;
    uint16_t    nImageHeight;
    
    uint16_t    nFrameRate;
    uint64_t    nBitRate;
    uint16_t    nGOPSize;
    uint16_t    nMaxBFrames;
    uint16_t    nTotalFrames;
} IOParam;

bool Parse_input_param(int argc, char *argv[], IOParam &io_param);

bool Open_file(IOParam &io_param);

void Close_file(IOParam &io_param);

int Read_yuv_data(CodecCtx &ctx, IOParam &io_param, int color_plane);

void write_out_yuv_frame(const CodecCtx &ctx, IOParam &in_out);

#endif
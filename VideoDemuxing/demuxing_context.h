#pragma once
#ifndef _DEMUXING_CONTEXT_H_
#define _DEMUXING_CONTEXT_H_

#include "video_demuxing_header.h"
#include "IO_file.h"

typedef struct 
{
    AVFormatContext *fmt_ctx;
    AVCodecContext  *video_dec_ctx, *audio_dec_ctx;
    AVStream        *video_stream, *audio_stream;
    AVFrame         *frame;
    AVPacket        pkt;

    int             video_stream_idx, audio_stream_idx;
    int             width, height;

    uint8_t         *video_dst_data[4];
    int             video_dst_linesize[4];
    int             video_dst_bufsize;
    enum AVPixelFormat  pix_fmt;   
} DemuxingVideoAudioContext;

int InitDemuxContext(IOFileName &files, DemuxingVideoAudioContext &va_ctx);

void CloseDemuxContext(IOFileName &files, DemuxingVideoAudioContext &va_ctx);

#endif
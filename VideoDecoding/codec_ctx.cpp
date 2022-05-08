#include "codec_ctx.h"

void InitCodecCtx(CodecCtx &codec_ctx, enum AVCodecID avcodec_id)
{
    codec_ctx.pkt = av_packet_alloc();
    if(!codec_ctx.pkt)
    {
        exit(1);
    }

    codec_ctx.codec = avcodec_find_decoder(avcodec_id);
    if (!codec_ctx.codec) 
    {
        fprintf(stderr, "Codec not found\n");
        exit(1);
    }
    
    codec_ctx.ctx = avcodec_alloc_context3(codec_ctx.codec);
    if(!codec_ctx.ctx)
    {
        fprintf(stderr, "Could not allocate video codec context\n");
        exit(1);
    }
    if (codec_ctx.codec->capabilities & AV_CODEC_CAP_TRUNCATED)
		codec_ctx.ctx->flags |= AV_CODEC_FLAG_TRUNCATED;

    codec_ctx.parser = av_parser_init(codec_ctx.codec->id);
    if (!codec_ctx.parser) 
    {
        fprintf(stderr, "parser not found\n");
        exit(1);
    }

    codec_ctx.frame = av_frame_alloc();
    if (!codec_ctx.frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }

    if (avcodec_open2(codec_ctx.ctx, codec_ctx.codec, NULL) < 0) 
    {
        fprintf(stderr, "Could not open codec\n");
        exit(1);
    }

    return;
}

void CloseCodecCtx(CodecCtx &codec_ctx)
{
    av_parser_close(codec_ctx.parser);
    avcodec_free_context(&(codec_ctx.ctx));
    av_frame_free(&(codec_ctx.frame));
    av_packet_free(&(codec_ctx.pkt));
}
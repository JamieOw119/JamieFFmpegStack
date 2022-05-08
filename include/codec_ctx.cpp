#include "codec_ctx.h"
#include "input_output.h"

void InitDecodeCodecCtx(CodecCtx &codec_ctx, enum AVCodecID avcodec_id)
{
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

    codec_ctx.pkt = av_packet_alloc();
    if(!codec_ctx.pkt)
    {
        fprintf(stderr, "Could not allocate video packet\n");
        exit(1);
    }

    if (avcodec_open2(codec_ctx.ctx, codec_ctx.codec, NULL) < 0) 
    {
        fprintf(stderr, "Could not open codec\n");
        exit(1);
    }

    return;
}

void InitEncodeCodecCtx(CodecCtx &codec_ctx, IOParam io_param, enum AVCodecID avcodec_id)
{
    codec_ctx.codec = avcodec_find_encoder(avcodec_id);
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
    SetEncodeCodecCtx(codec_ctx, io_param);

    codec_ctx.frame = av_frame_alloc();
    if (!codec_ctx.frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }
    codec_ctx.frame->format = codec_ctx.ctx->pix_fmt;
    codec_ctx.frame->width = codec_ctx.ctx->width;
    codec_ctx.frame->height = codec_ctx.ctx->height;

    codec_ctx.pkt = av_packet_alloc();
    if(!codec_ctx.pkt)
    {
        fprintf(stderr, "Could not allocate video packet\n");
        exit(1);
    }

    if (avcodec_open2(codec_ctx.ctx, codec_ctx.codec, NULL) < 0) 
    {
        fprintf(stderr, "Could not open codec\n");
        exit(1);
    }

    if (av_frame_get_buffer(codec_ctx.frame, 0) < 0) 
	{
		fprintf(stderr, "Could not allocate the video frame data\n");
		exit(1);
	}

    return;
}

void SetEncodeCodecCtx(CodecCtx &codec_ctx, IOParam io_param)
{
    codec_ctx.ctx->bit_rate = io_param.n_bit_rate;  
    codec_ctx.ctx->width = io_param.n_image_width;
    codec_ctx.ctx->height = io_param.n_image_height;
    codec_ctx.ctx->time_base = (AVRational){1, 25};
    codec_ctx.ctx->framerate = (AVRational){25, 1};
    codec_ctx.ctx->gop_size = io_param.n_GOP_size;
    codec_ctx.ctx->max_b_frames = io_param.n_max_b_frames;
    codec_ctx.ctx->pix_fmt = AV_PIX_FMT_YUV420P;
    if (codec_ctx.codec->id == AV_CODEC_ID_H264)
        av_opt_set(codec_ctx.ctx->priv_data, "preset", "slow", 0);
}

void CloseCodecCtx(CodecCtx &codec_ctx, bool isDecoding)
{
    if(isDecoding)
        av_parser_close(codec_ctx.parser);
    avcodec_free_context(&(codec_ctx.ctx));
    av_frame_free(&(codec_ctx.frame));
    av_packet_free(&(codec_ctx.pkt));
}
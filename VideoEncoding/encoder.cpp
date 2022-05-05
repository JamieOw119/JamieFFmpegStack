#include "encoder.h"

void setContext(CodecCtx &ctx, IOParam io_param)
{
    ctx.c->bit_rate = io_param.nBitRate;
    ctx.c->width = io_param.nImageWidth;
    ctx.c->height = io_param.nImageHeight;

    AVRational rational = {1, 25};
    ctx.c->time_base = rational;

    ctx.c->gop_size = io_param.nGOPSize;
    ctx.c->max_b_frames = io_param.nMaxBFrames;
    ctx.c->pix_fmt = AV_PIX_FMT_YUV420P;

    av_opt_set(ctx.c->priv_data, "preset", "slow", 0);
}

bool Open_encoder(CodecCtx &ctx, IOParam io_param)
{
    int ret;
    avcodec_register_all();

    ctx.codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if(!ctx.codec)
    {
        fprintf(stderr, "Codec not found\n");
        return false;
    }

    setContext(ctx, io_param);

    if(avcodec_open2(ctx.c, ctx.codec, NULL) < 0)
    {
        fprintf(stderr, "Could not open codec\n");
		exit(1);
    }

    ctx.frame = av_frame_alloc();						//分配AVFrame对象
	if (!ctx.frame) 
	{
        fprintf(stderr, "Could not allocate video frame\n");
        return false;
    }

    ctx.frame->format = ctx.c->pix_fmt;
    ctx.frame->width = ctx.c->width;
    ctx.frame->height = ctx.c->height;

    ret = av_image_alloc(ctx.frame->data, ctx.frame->linesize, ctx.c->width, ctx.c->height, ctx.c->pix_fmt, 32);
    if (ret < 0) 
	{
		fprintf(stderr, "Could not allocate raw picture buffer\n");
		return false;
	}

	return true;
}

void Close_encoder(CodecCtx &ctx)
{
    avcodec_close(ctx.c);
    av_free(ctx.c);
    av_freep(&(ctx.frame->data[0]));
    av_frame_free(&(ctx.frame));
}
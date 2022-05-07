#include "decoder.h"

bool Open_decoder(CodecCtx &ctx)
{
    ctx.pkt = av_packet_alloc();
	if(!ctx.pkt)
	{
		fprintf(stderr, "Could not allocate video codec packet\n");
		return false;
	}

    ctx.pCodec = avcodec_find_decoder(AV_CODEC_ID_H264);
    if (!ctx.pCodec) 
	{
		fprintf(stderr, "Codec not found\n");
		return false;
	}
    
    ctx.pCodecContext = avcodec_alloc_context3(ctx.pCodec);
    if (!ctx.pCodecContext)
	{
		fprintf(stderr, "Could not allocate video codec context\n");
		return false;
	}

    if(ctx.pCodec->capabilities & AV_CODEC_CAP_TRUNCATED)
    {
        ctx.pCodecContext->flags |= AV_CODEC_FLAG_TRUNCATED;
    }

    ctx.pCodecParserCtx = av_parser_init(AV_CODEC_ID_H264);
	if (!ctx.pCodecParserCtx)
	{
		printf("Could not allocate video parser context\n");
		return false;
	}

    if (avcodec_open2(ctx.pCodecContext, ctx.pCodec, NULL) < 0)
	{
		fprintf(stderr, "Could not open codec\n");
		return false;
	}

    ctx.frame = av_frame_alloc();
	if (!ctx.frame) 
	{
		fprintf(stderr, "Could not allocate video frame\n");
		return false;
	}

	return true;
}

void Close_decoder(CodecCtx &ctx)
{
	avcodec_free_context(&(ctx.pCodecContext));
	av_parser_close(ctx.pCodecParserCtx);
	av_packet_free(&(ctx.pkt));
	av_frame_free(&(ctx.frame));
}

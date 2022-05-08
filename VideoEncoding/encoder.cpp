#include "encoder.h"

void Encode(CodecCtx &codec_ctx, IOParam &io_param)
{
    int ret, x, y;
    int duration = (int)(io_param.n_total_frames / codec_ctx.ctx->framerate.den);
    for(int i = 0; i < duration; i++)
    {
        fflush(stdout);
        ret = av_frame_make_writable(codec_ctx.frame);
        if(ret < 0)
            exit(1);
        // Y
        for(y=0;  y < codec_ctx.ctx->height; y++)
        {
            for(x=0;  x < codec_ctx.ctx->width; x++)
            {
                codec_ctx.frame->data[0][y * codec_ctx.frame->linesize[0] + x] = 128 + y + i * 3;
            }
        }
        // Cb and Cr
        for(y=0;  y < codec_ctx.ctx->height>>1; y++)
        {
            for(x=0;  x < codec_ctx.ctx->width>>1; x++)
            {
                codec_ctx.frame->data[1][y * codec_ctx.frame->linesize[1] + x] = 128 + y + i * 2;
                codec_ctx.frame->data[2][y * codec_ctx.frame->linesize[2] + x] = 64 + x + i * 5;
            }
        }
        codec_ctx.frame->pts = i;
        EncodeCore(codec_ctx, io_param);
    }

    // flush the encoder
    EncodeCore(codec_ctx, io_param);

    uint8_t endcode[] = { 0, 0, 1, 0xb7 };
    if (codec_ctx.codec->id == AV_CODEC_ID_MPEG1VIDEO || codec_ctx.codec->id == AV_CODEC_ID_MPEG2VIDEO)
        fwrite(endcode, 1, sizeof(endcode), io_param.p_out);
    
    return;
}

void EncodeCore(CodecCtx &codec_ctx, IOParam &io_param)
{
    int ret;
    if(codec_ctx.frame)
    {
        printf("Send frame %8ld\n", codec_ctx.frame->pts);
    }

    ret = avcodec_send_frame(codec_ctx.ctx, codec_ctx.frame);
    if (ret < 0) {
        fprintf(stderr, "Error sending a frame for encoding\n");
        exit(1);
    }

    while (ret >= 0) 
    {
        ret = avcodec_receive_packet(codec_ctx.ctx, codec_ctx.pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return;
        else if (ret < 0) {
            fprintf(stderr, "Error during encoding\n");
            exit(1);
        }
 
        printf("Write packet %8ld (size=%d)\n", codec_ctx.pkt->pts, codec_ctx.pkt->size);
        fwrite(codec_ctx.pkt->data, 1, codec_ctx.pkt->size, io_param.p_out);
        av_packet_unref(codec_ctx.pkt);
    }

    return;
}

int ReadInYUVData(CodecCtx &codec_ctx, IOParam &io_param, int color_plane)
{
    int frame_height	= color_plane == 0? codec_ctx.frame->height : codec_ctx.frame->height / 2;
	int frame_width		= color_plane == 0? codec_ctx.frame->width : codec_ctx.frame->width / 2;
    int frame_size		= frame_width * frame_height;
	int frame_stride	= codec_ctx.frame->linesize[color_plane];

    if(frame_width == frame_stride)
    {
        fread(codec_ctx.frame->data[color_plane], 1, frame_size, io_param.p_in);
    }
    else
    {
        for (int row_idx = 0; row_idx < frame_height; row_idx++)
		{
			fread(codec_ctx.frame->data[color_plane] + row_idx * frame_stride, 1, frame_width, io_param.p_in);
		}
    }
    return frame_size;
}
#include "decoder.h"
#include "input_output.h"

void Decode(CodecCtx &codec_ctx, IOParam &io_param)
{
    uint8_t inbuf[INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];
    uint8_t *data;
    size_t data_size;
    int eof;
    int ret;

    do
    {
        data_size = fread(inbuf, 1, INBUF_SIZE, io_param.p_in);
        if(ferror(io_param.p_in))
            break;
        eof = !data_size;

        data = inbuf;
        while(data_size > 0 || eof)
        {
            ret = av_parser_parse2(codec_ctx.parser, codec_ctx.ctx, 
                                    &(codec_ctx.pkt->data), &(codec_ctx.pkt->size),
                                    data, data_size, 
                                    AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
            if (ret < 0) 
            {
                fprintf(stderr, "Error while parsing\n");
                exit(1);
            }
            data      += ret;
            data_size -= ret;
 
            if (codec_ctx.pkt->size)
            {
                DecodeCore(codec_ctx, io_param);
            }
            else if (eof)
                break;
        }
    } while (!eof);
    return;
}

void DecodeCore(CodecCtx &codec_ctx, IOParam &io_param)
{
    int ret;

    ret = avcodec_send_packet(codec_ctx.ctx, codec_ctx.pkt);
    if (ret < 0) {
        fprintf(stderr, "Error sending a packet for decoding\n");
        exit(1);
    }

    while(ret >= 0)
    {
        ret = avcodec_receive_frame(codec_ctx.ctx, codec_ctx.frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return;
        else if (ret < 0) 
        {
            fprintf(stderr, "Error during decoding\n");
            exit(1);
        }

        printf("saving frame %3d\n", codec_ctx.ctx->frame_number);
        fflush(stdout);

        WriteOutYUVFile(codec_ctx, io_param);
    }
    return;
}

void WriteOutYUVFile(CodecCtx &codec_ctx, IOParam &io_param)
{
	uint8_t **pBuf	= codec_ctx.frame->data;
	int*	pStride = codec_ctx.frame->linesize;
	
	for (int color_idx = 0; color_idx < 3; color_idx++)
	{
		int		nWidth	= color_idx == 0 ? codec_ctx.frame->width : codec_ctx.frame->width / 2;
		int		nHeight = color_idx == 0 ? codec_ctx.frame->height : codec_ctx.frame->height / 2;
		for(int idx=0;idx < nHeight; idx++)
		{
			fwrite(pBuf[color_idx],1, nWidth, io_param.p_out);
			pBuf[color_idx] += pStride[color_idx];
		}
		fflush(io_param.p_out);
	}
    return;
}
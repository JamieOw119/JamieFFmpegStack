#include <stdio.h>

#include "input_output.h"
#include "decoder.h"

void hello()
{
	printf("*********************************\n");
	printf("VideoDecoding: A FFmpeg SDK demo.\n");
	printf("*********************************\n");
	printf("=================================\nCompulsory Paramaters:\n");
	printf("Input YUV file name");
	printf("\tOutput stream file name\n");
	printf("*********************************\n");
}

int main(int argc, char **argv)
{
    uint8_t *pDataPtr = NULL;
    int uDataSize = 0;
    int got_picture, len;

    CodecCtx ctx;
    IOParam inputoutput;

    hello();

    Parse_input_param(argc, argv, inputoutput);

    Open_file(inputoutput);

    uint8_t inbuf[INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];
	
	memset(inbuf + INBUF_SIZE, 0, AV_INPUT_BUFFER_PADDING_SIZE);
	
	printf("Decode video file %s to %s\n", argv[1], argv[2]);

	Open_decoder(ctx);

    while(1)
    {
        uDataSize = fread(inbuf, 1, INBUF_SIZE, inputoutput.pFin);
        if(0 == uDataSize)
        {
            break;
        }
        pDataPtr = inbuf;
        while(uDataSize > 0)
        {
            len = av_parser_parse2(ctx.pCodecParserCtx, ctx.pCodecContext, 
										&(ctx.pkt.data), &(ctx.pkt.size), 
										pDataPtr, uDataSize, 
										AV_NOPTS_VALUE, AV_NOPTS_VALUE, AV_NOPTS_VALUE);
            pDataPtr += len;
            uDataSize -= len;

            if (0 == ctx.pkt.size)
			{
				continue;
			}

            printf("Parse 1 packet. Packet pts: %d.\n", ctx.pkt.pts);

			int ret = avcodec_decode_video2(ctx.pCodecContext, ctx.frame, &got_picture, &(ctx.pkt));
			if (ret < 0) 
			{
				printf("Decode Error.\n");
				return ret;
			}

            if (got_picture) 
			{
				write_out_yuv_frame(ctx, inputoutput);
				printf("Succeed to decode 1 frame! Frame pts: %d\n", ctx.frame->pts);
			}
        }
    }

    Close_file(inputoutput);
	Close_decoder(ctx);

	return 0;
}
#include <stdio.h>
#include "../include/input_output.h"
#include "video_encoding_header.h"
#include "encoder.h"


void hello()
{
	printf("*********************************\n");
	printf("VideoEncoding: A FFmpeg SDK demo.\nDeveloped by Yin Wenjie\n");
	printf("*********************************\n");
	printf("=================================\nCompulsory Paramaters:\n");
	printf("\t-i:\tInput YUV file name\n");
	printf("\t-o:\tOutput stream file name\n");
	printf("\t-w:\tInput frame width\n");
	printf("\t-h:\tInput frame height\n");
	printf("\t-br:\tInput bit rate\n");
	printf("\t-tf:\tTotal frames to encode\n");
	printf("=================================\nOptional Paramaters:\n");
	printf("\t-fr:\tFrame rate\n");
	printf("\t-gs:\tGOP size\n");
	printf("\t-mbf:\tMax B Frames\n");
	printf("*********************************\n");
}

int main(int argc, char *argv[])
{
    hello();

    IOParam io_param;
    if(Parse_input_param(argc, argv, io_param))
    {
        printf("Error: Incomplete input parameters. Please check the command line.");
    }

    CodecCtx ctx = {NULL, NULL, NULL};
    int frameIdx, packetIdx = 0, ret, got_output;

    Open_file(io_param);
    Open_encoder(ctx, io_param);

    for(frameIdx = 0; frameIdx < io_param.nTotalFrames; frameIdx++)
    {
        av_init_packet(&(ctx.pkt));				
		ctx.pkt.data = NULL;	
        ctx.pkt.size = 0;

        fflush(stdout);
        Read_yuv_data(ctx, io_param, 0);
        Read_yuv_data(ctx, io_param, 1);
        Read_yuv_data(ctx, io_param, 2);

        ctx.frame->pts = frameIdx;

        ret = avcodec_encode_video2(ctx.c, &(ctx.pkt), ctx.frame, &got_output);
		if (ret < 0) 
		{
			fprintf(stderr, "Error encoding frame\n");
			exit(1);
		}
        printf("Encode frame index: %d, frame pts: %d.\n", frameIdx, ctx.frame->pts);
		if (got_output) 
		{
			printf("Write packets %3d (size=%5d). Packet pts: %d\n", packetIdx++, ctx.pkt.size, ctx.pkt.pts);
			fwrite(ctx.pkt.data, 1, ctx.pkt.size, io_param.pFout);
			av_packet_unref(&(ctx.pkt));
		}
    }    

    for (got_output = 1; got_output; frameIdx++) 
	{
		fflush(stdout);

		ret = avcodec_encode_video2(ctx.c, &(ctx.pkt), NULL, &got_output);
		if (ret < 0)
		{
			fprintf(stderr, "Error encoding frame\n");
			exit(1);
		}

		if (got_output) 
		{
			printf("Cached frames: Write packets %3d (size=%5d). Packet pts: %d\n", packetIdx++, ctx.pkt.size, ctx.pkt.pts);
			fwrite(ctx.pkt.data, 1, ctx.pkt.size, io_param.pFout);
			av_packet_unref(&(ctx.pkt));
		}
	} 

    Close_file(io_param);
    Close_encoder(ctx);
    
    return 0;
}

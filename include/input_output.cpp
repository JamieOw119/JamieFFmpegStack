#include <string.h>
#include <stdlib.h>
#include "input_output.h"

bool Parse_input_param(int argc, char *argv[], IOParam &io_param)
{
    bool bNameInFound = false;
    bool bNameOutFound = false;
    bool bWidthFound = false;
    bool bHeightFound = false;
    bool bBitrateFound = false;
    bool bTotalFrames = false;

    io_param.nFrameRate = 25;
    io_param.nGOPSize = 10;
    io_param.nMaxBFrames = 1;

    for(int idx = 1; idx < argc; idx ++)
    {
        if(!strcasecmp(argv[idx], "-i"))
        {
            io_param.pNameIn = argv[idx+1];
			bNameInFound = true;
        }
        if (!strcasecmp(argv[idx], "-o"))
		{
			io_param.pNameOut = argv[idx+1];
			bNameOutFound = true;
		}
		if (!strcasecmp(argv[idx], "-w"))
		{
			io_param.nImageWidth = atoi(argv[idx+1]);
			bWidthFound = true;
		}
		if (!strcasecmp(argv[idx], "-h"))
		{
			io_param.nImageHeight = atoi(argv[idx+1]);
			bHeightFound = true;
		}
		if (!strcasecmp(argv[idx], "-br"))
		{
			io_param.nBitRate = atol(argv[idx+1]);
			bBitrateFound = true;
		}
	    if (!strcasecmp(argv[idx], "-tf"))
		{
			io_param.nTotalFrames = atoi(argv[idx+1]);
			bTotalFrames = true;
		}
		if (!strcasecmp(argv[idx], "-fr"))
		{
			io_param.nFrameRate = atoi(argv[idx+1]);
		}
	    if (!strcasecmp(argv[idx], "-gs"))
		{
			io_param.nGOPSize = atoi(argv[idx+1]);
		}
		if (!strcasecmp(argv[idx], "-mbs"))
		{
			io_param.nMaxBFrames = atoi(argv[idx+1]);
		}
    }

    if (!bNameInFound)
	{
		printf("Error: Cannot find input file name.\n");
		return false;
	}
	if (!bNameOutFound)
	{
		printf("Error: Cannot find output file name.\n");
		return false;
	}
	if (!bWidthFound)
	{
		printf("Error: Cannot find image width.\n");
		return false;
	}
	if (!bHeightFound)
	{
		printf("Error: Cannot find image height.\n");
		return false;
	}
	if (!bBitrateFound)
	{
		printf("Error: Cannot find bitrate.\n");
		return false;
	}

	printf("Paramaters parsing OK!\n");
	return true;
}

bool Open_file(IOParam &io_param)
{
    io_param.pFin = fopen(io_param.pNameIn, "rb");
    if(!(io_param.pFin))
    {
        fprintf(stderr, "Could not open %s\n", io_param.pNameIn);
		return false;
    }

    io_param.pFout = fopen(io_param.pNameOut, "wb");
	if (!(io_param.pFout))
	{
		fprintf(stderr, "Could not open %s\n", io_param.pNameOut);
		return false;
	}

	return true;
}

void Close_file(IOParam &io_param)
{
    fclose(io_param.pFin);
    fclose(io_param.pFout);
}

int Read_yuv_data(CodecCtx &ctx, IOParam &io_param, int color_plane)
{
    int frame_height	= color_plane == 0? ctx.frame->height : ctx.frame->height / 2;
	int frame_width		= color_plane == 0? ctx.frame->width : ctx.frame->width / 2;
    int frame_size      = frame_width * frame_height;
    int frame_stride    = ctx.frame->linesize[color_plane];

    if(frame_width == frame_stride)
    {
        fread(ctx.frame->data[color_plane], 1, frame_size, io_param.pFin);
    }
    else
    {
        for(int row_idx = 0; row_idx < frame_height; row_idx ++)
        {
            fread(ctx.frame->data[color_plane] + row_idx * frame_stride, 1, frame_size, io_param.pFin);
        }
    }
    return frame_size;
}

void write_out_yuv_frame(CodecCtx &ctx, IOParam &in_out)
{
    uint8_t **pBuf	= ctx.frame->data;
	int*	pStride = ctx.frame->linesize;

    for(int color_idx = 0; color_idx < 3; color_idx ++)
    {
        int		nWidth	= color_idx == 0 ? ctx.frame->width : ctx.frame->width / 2;
		int		nHeight = color_idx == 0 ? ctx.frame->height : ctx.frame->height / 2;
		for(int idx=0;idx < nHeight; idx++)
		{
			fwrite(pBuf[color_idx],1, nWidth, in_out.pFout);
			pBuf[color_idx] += pStride[color_idx];
		}
		fflush(in_out.pFout);
    }
}
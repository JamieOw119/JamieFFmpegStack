#include "input_output.h"

void ParseForDecode(int argc, char **argv, IOParam &io_param)
{
    io_param.in_filename = argv[1];
    io_param.out_filename = argv[2];
}

void ParseForEncode(int argc, char **argv, IOParam &io_param)
{
    bool bNameInFound = false;
    bool bNameOutFound = false;
    bool bWidthFound = false;
    bool bHeightFound = false;
    bool bBitrateFound = false;
    bool bTotalFrames = false;

    for (int idx = 1; idx < argc; idx++)
	{
		if (!strcasecmp(argv[idx], "-i"))
		{
			io_param.in_filename = argv[idx+1];
			bNameInFound = true;
		} 
		else if (!strcasecmp(argv[idx], "-o"))
		{
			io_param.out_filename = argv[idx+1];
			bNameOutFound = true;
		}
		else if (!strcasecmp(argv[idx], "-w"))
		{
			io_param.n_image_width = atoi(argv[idx+1]);
			bWidthFound = true;
		}
		else if (!strcasecmp(argv[idx], "-h"))
		{
			io_param.n_image_height = atoi(argv[idx+1]);
			bHeightFound = true;
		}
		else if (!strcasecmp(argv[idx], "-br"))
		{
			io_param.n_bit_rate = atol(argv[idx+1]);
			bBitrateFound = true;
		}
		else if (!strcasecmp(argv[idx], "-tf"))
		{
			io_param.n_total_frames = atoi(argv[idx+1]);
			bTotalFrames = true;
		}
		else if (!strcasecmp(argv[idx], "-fr"))
		{
			io_param.n_frame_rate = atoi(argv[idx+1]);
		}
		else if (!strcasecmp(argv[idx], "-gs"))
		{
			io_param.n_GOP_size = atoi(argv[idx+1]);
		}
		else if (!strcasecmp(argv[idx], "-mbs"))
		{
			io_param.n_max_b_frames = atoi(argv[idx+1]);
		}
	}

	if (!bNameInFound)
	{
		printf("Error: Cannot find input file name.\n");
		exit(1);
	}
	if (!bNameOutFound)
	{
		printf("Error: Cannot find output file name.\n");
		exit(1);
	}
	if (!bWidthFound)
	{
		printf("Error: Cannot find image width.\n");
		exit(1);
	}
	if (!bHeightFound)
	{
		printf("Error: Cannot find image height.\n");
		exit(1);
	}
	if (!bBitrateFound)
	{
		printf("Error: Cannot find bitrate.\n");
		exit(1);
	}

	printf("Paramaters parsing OK!\n");
}

bool OpenFiles(IOParam &io_param)
{
    io_param.p_in = fopen(io_param.in_filename, "rb");
    if(!io_param.p_in)
    {
        printf("Could not open %s\n", io_param.in_filename);
		return false;
    }

    io_param.p_out = fopen(io_param.out_filename, "wb");
    if(!io_param.p_out)
    {
        printf("Could not open %s\n", io_param.out_filename);
		return false;
    }
    
    return true;
}

void CloseFiles(IOParam &io_param)
{
    fclose(io_param.p_in);
    fclose(io_param.p_out);
}
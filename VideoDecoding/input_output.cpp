#include "input_output.h"

void Parse(int argc, char **argv, IOParam &io_param)
{
    io_param.in_filename = argv[1];
    io_param.out_filename = argv[2];
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
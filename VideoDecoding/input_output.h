#pragma once
#ifndef _INPUT_OUTPUT_H_
#define _INPUT_OUTPUT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INBUF_SIZE 4096

typedef struct 
{
    FILE *p_in;
    FILE *p_out;
    char *in_filename;
    char *out_filename;
} IOParam;

void Parse(int argc, char **argv, IOParam &io_param);

bool OpenFiles(IOParam &io_param);

void CloseFiles(IOParam &io_param);

#endif
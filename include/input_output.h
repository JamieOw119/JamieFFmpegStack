#pragma once
#ifndef _INPUT_OUTPUT_H_
#define _INPUT_OUTPUT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define INBUF_SIZE 4096

// typedef unsigned short  	uint16_t;
// typedef unsigned long long	uint64_t;

typedef struct 
{
    FILE *p_in;
    FILE *p_out;
    char *in_filename;
    char *out_filename;

    uint16_t n_image_width;
	uint16_t n_image_height;

	uint16_t n_frame_rate;
	uint64_t n_bit_rate;
	uint16_t n_GOP_size;
	uint16_t n_max_b_frames;
	uint16_t n_total_frames;
} IOParam;

void ParseForDecode(int argc, char **argv, IOParam &io_param);

void ParseForEncode(int argc, char **argv, IOParam &io_param);

bool OpenFiles(IOParam &io_param);

void CloseFiles(IOParam &io_param);

#endif
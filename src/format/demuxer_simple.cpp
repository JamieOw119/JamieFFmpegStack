#include <stdio.h>

#define __STDC_CONSTANT_MACROS

#ifdef _WIN32
//Windows
extern "C"
{
#include "libavformat/avformat.h"
};
#else
//Linux...
#ifdef __cplusplus
extern "C"
{
#endif
#include <libavformat/avformat.h>
#ifdef __cplusplus
};
#endif
#endif


//'1': Use H.264 Bitstream Filter 
#define USE_H264BSF 1

int main(int argc, char* argv[])
{
	AVFormatContext *ifmt_ctx = NULL;
	AVPacket pkt;
	int ret, i;
	int videoindex=-1,audioindex=-1;
	const char *in_filename  = "cuc_ieschool.flv";//Input file URL
	const char *out_filename_v = "cuc_ieschool.h264";//Output file URL
	const char *out_filename_a = "cuc_ieschool.mp3";

	av_register_all();
	//Input
	if ((ret = avformat_open_input(&ifmt_ctx, in_filename, 0, 0)) < 0) {
		printf( "Could not open input file.");
		return -1;
	}
	if ((ret = avformat_find_stream_info(ifmt_ctx, 0)) < 0) {
		printf( "Failed to retrieve input stream information");
		return -1;
	}

	videoindex=-1;
	for(i=0; i<ifmt_ctx->nb_streams; i++) {
		if(ifmt_ctx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO){
			videoindex=i;
		}else if(ifmt_ctx->streams[i]->codec->codec_type==AVMEDIA_TYPE_AUDIO){
			audioindex=i;
		}
	}
	//Dump Format------------------
	printf("\nInput Video===========================\n");
	av_dump_format(ifmt_ctx, 0, in_filename, 0);
	printf("\n======================================\n");

	FILE *fp_audio=fopen(out_filename_a,"wb+");  
	FILE *fp_video=fopen(out_filename_v,"wb+");  

	/*
	FIX: H.264 in some container format (FLV, MP4, MKV etc.) need 
	"h264_mp4toannexb" bitstream filter (BSF)
	  *Add SPS,PPS in front of IDR frame
	  *Add start code ("0,0,0,1") in front of NALU
	H.264 in some container (MPEG2TS) don't need this BSF.
	*/
#if USE_H264BSF
	AVBitStreamFilterContext* h264bsfc =  av_bitstream_filter_init("h264_mp4toannexb"); 
#endif

	while(av_read_frame(ifmt_ctx, &pkt)>=0){
		if(pkt.stream_index==videoindex){
#if USE_H264BSF
			av_bitstream_filter_filter(h264bsfc, ifmt_ctx->streams[videoindex]->codec, NULL, &pkt.data, &pkt.size, pkt.data, pkt.size, 0);
#endif
			printf("Write Video Packet. size:%d\tpts:%lld\n",pkt.size,pkt.pts);
			fwrite(pkt.data,1,pkt.size,fp_video);
		}else if(pkt.stream_index==audioindex){
			/*
			AAC in some container format (FLV, MP4, MKV etc.) need to add 7 Bytes
			ADTS Header in front of AVPacket data manually.
			Other Audio Codec (MP3...) works well.
			*/
			printf("Write Audio Packet. size:%d\tpts:%lld\n",pkt.size,pkt.pts);
			fwrite(pkt.data,1,pkt.size,fp_audio);
		}
		av_free_packet(&pkt);
	}

#if USE_H264BSF
	av_bitstream_filter_close(h264bsfc);  
#endif

	fclose(fp_video);
	fclose(fp_audio);

	avformat_close_input(&ifmt_ctx);

	if (ret < 0 && ret != AVERROR_EOF) {
		printf( "Error occurred.\n");
		return -1;
	}
	return 0;
}



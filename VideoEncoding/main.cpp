#include "input_output.h"
#include "codec_ctx.h"
#include "encoder.h"

int main(int argc, char **argv)
{
    IOParam io_param;
    ParseForEncode(argc, argv, io_param);
    OpenFiles(io_param);

    CodecCtx codec_ctx;
    InitEncodeCodecCtx(codec_ctx, io_param, AV_CODEC_ID_H264);
    printf("%d %d\n", codec_ctx.ctx->time_base.den, codec_ctx.ctx->time_base.num);

    printf("Decode video file %s to %s\n", argv[1], argv[2]);
    Encode(codec_ctx, io_param);

    CloseFiles(io_param);
    CloseCodecCtx(codec_ctx, false);  
    return 0;
}

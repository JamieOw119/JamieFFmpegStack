#include "input_output.h"
#include "codec_ctx.h"
#include "decoder.h"

int main(int argc, char **argv)
{
    IOParam io_param;
    ParseForDecode(argc, argv, io_param);
    OpenFiles(io_param);

    CodecCtx codec_ctx;
    InitDecodeCodecCtx(codec_ctx, AV_CODEC_ID_H264);

    printf("Decode video file %s to %s\n", argv[1], argv[2]);
    Decode(codec_ctx, io_param);

    CloseFiles(io_param);
    CloseCodecCtx(codec_ctx, true);
    return 0;
}

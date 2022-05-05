# Install FFmpeg in Linux

## 1. Environment

- OS: Ubuntu 20.04 x64
- VM: Tencent Cloud Server (apt-get uses Tencent's own default source)

## 2. Dependency

- [libdav1d4](https://packages.debian.org/bullseye/libdav1d4)
- [libdav1d-dev](https://packages.debian.org/bullseye/libdav1d-dev)

## 3. Install FFmpeg

1. `./install_ffmpeg.sh`
    > If the network condition does not increase, you can download the ffmpeg source compression package in advance.

2. move or copy `~/bin/ffmpeg` `~/bin/ffprobe` to `/usr/bin` 

## 4. Install dependent libraries

1. `./install_dev_libs.sh`

2. If step 1 failed, there are two alternatives:

    2.1 move or copy `~/ffmpeg_build/lib` to your dictionary and remember to add these static libraries when compiling (use '-L' argument).

    2.2 move or copy `~/ffmpeg_build/include` to your dictionary and remember to add these header files when compiling (use '-I' argument). 

Author uses 2.1 :-)


# Install and Using FFmpeg in Linux

## 1.1: Install FFmpeg in Linux (Source Code Compilation)

### 1. Environment

- OS: Ubuntu 20.04 x64
- VM: Tencent Cloud Server (apt-get uses Tencent's default source)

```
mkdir -p ~/ffmpeg_sources ~/ffmpeg_build ~/bin
```

### 2. Dependency

```
sudo apt-get update -qq && sudo apt-get -y install \
  autoconf \
  automake \
  build-essential \
  cmake \
  git-core \
  meson \
  ninja-build \
  pkg-config \
  texinfo \
  wget \
  yasm \
  libnuma-dev \
  zlib1g-dev \
  liblzma-dev \
  libbz2-dev \
  libvorbis-dev \
  libass-dev \
  libfreetype6-dev \
  libgnutls28-dev \
  libunistring-dev \
  libtool
```
  
### 3. Install Video Codec Libraries

1. nasm

```
cd ~/ffmpeg_sources && \
wget https://www.nasm.us/pub/nasm/releasebuilds/2.15.05/nasm-2.15.05.tar.bz2 && \
tar xjvf nasm-2.15.05.tar.bz2 && \
cd nasm-2.15.05 && \
./autogen.sh && \
PATH="$HOME/bin:$PATH" ./configure --prefix="$HOME/ffmpeg_build" --bindir="$HOME/bin" && \
make && \
make install
```

2. libx264

```
cd ~/ffmpeg_sources && \
git -C x264 pull 2> /dev/null || git clone --depth 1 https://code.videolan.org/videolan/x264.git && \
cd x264 && \
PATH="$HOME/bin:$PATH" PKG_CONFIG_PATH="$HOME/ffmpeg_build/lib/pkgconfig" ./configure --prefix="$HOME/ffmpeg_build" --bindir="$HOME/bin" --enable-static --enable-pic && \
PATH="$HOME/bin:$PATH" make && \
make install
```

3. libx265

```
cd ~/ffmpeg_sources && \
wget -O x265.tar.bz2 https://bitbucket.org/multicoreware/x265_git/get/master.tar.bz2 && \
tar xjvf x265.tar.bz2 && \
cd multicoreware*/build/linux && \
PATH="$HOME/bin:$PATH" cmake -G "Unix Makefiles" -DCMAKE_INSTALL_PREFIX="$HOME/ffmpeg_build" -DENABLE_SHARED=off ../../source && \
PATH="$HOME/bin:$PATH" make && \
make install
```

4. libvpx

```
cd ~/ffmpeg_sources && \
git -C libvpx pull 2> /dev/null || git clone --depth 1 https://chromium.googlesource.com/webm/libvpx.git && \
cd libvpx && \
PATH="$HOME/bin:$PATH" ./configure --prefix="$HOME/ffmpeg_build" --disable-examples --disable-unit-tests --enable-vp9-highbitdepth --as=yasm && \
PATH="$HOME/bin:$PATH" make && \
make install
```

5. libfdk-aac

```
cd ~/ffmpeg_sources && \
git -C fdk-aac pull 2> /dev/null || git clone --depth 1 https://github.com/mstorsjo/fdk-aac && \
cd fdk-aac && \
autoreconf -fiv && \
./configure --prefix="$HOME/ffmpeg_build" --bindir="$HOME/bin" --disable-shared && \
make && \
make install
```

6. libaom

```
cd ~/ffmpeg_sources && \
git -C aom pull 2> /dev/null || git clone --depth 1 https://aomedia.googlesource.com/aom && \
mkdir -p aom_build && \
cd aom_build && \
PATH="$HOME/bin:$PATH" cmake -G "Unix Makefiles" -DCMAKE_INSTALL_PREFIX="$HOME/ffmpeg_build" -DENABLE_TESTS=OFF -DENABLE_NASM=on ../aom && \
PATH="$HOME/bin:$PATH" make && \
make install
```

7. libsvtav1

```
cd ~/ffmpeg_sources && \
git -C SVT-AV1 pull 2> /dev/null || git clone https://gitlab.com/AOMediaCodec/SVT-AV1.git && \
mkdir -p SVT-AV1/Build && \
cd SVT-AV1/build && \
PATH="$HOME/bin:$PATH" cmake -G "Unix Makefiles" -DCMAKE_INSTALL_PREFIX="$HOME/ffmpeg_build" -DCMAKE_BUILD_TYPE=Release -DBUILD_DEC=OFF -DBUILD_SHARED_LIBS=OFF .. && \
PATH="$HOME/bin:$PATH" make && \
make install
```

> If you want to install libdav1d and libvamf, install meson and setup nasm.

```
sudo apt-get install python3-pip && \
pip3 install --user meson
cp ~/bin/nasm /usr/bin/
```

8. libdav1d

```
cd ~/ffmpeg_sources && \
git -C dav1d pull 2> /dev/null || git clone --depth 1 https://code.videolan.org/videolan/dav1d.git && \
mkdir -p dav1d/build && \
cd dav1d/build && \
meson setup -Denable_tools=false -Denable_tests=false --default-library=static .. --prefix "$HOME/ffmpeg_build" --libdir="$HOME/ffmpeg_build/lib" && \
ninja && \
ninja install
```

9. libvmaf

```
cd ~/ffmpeg_sources && \
wget https://github.com/Netflix/vmaf/archive/v2.1.1.tar.gz && \cd ../
tar xvf v2.1.1.tar.gz && \
mkdir -p vmaf-2.1.1/libvmaf/build &&\
cd vmaf-2.1.1/libvmaf/build && \
meson setup -Denable_tests=false -Denable_docs=false --buildtype=release --default-library=static .. --prefix "$HOME/ffmpeg_build" --libdir="$HOME/ffmpeg_build/lib" && \
ninja && \
ninja install
```

### 4. Install Audio Codec Libraries

1. libopus

```
cd ~/ffmpeg_sources && \
git -C opus pull 2> /dev/null || git clone --depth 1 https://github.com/xiph/opus.git && \
cd opus && \
./autogen.sh && \
./configure --prefix="$HOME/ffmpeg_build" --bindir="$HOME/bin" --disable-shared && \
make && \
make install
```

2. libmp3lame

```
cd ~/ffmpeg_sources && \
wget -O lame-3.100.tar.gz https://sourceforge.net/projects/lame/files/lame/3.100/lame-3.100.tar.gz/download
tar zxvf lame-3.100.tar.gz
cd lame-3.100 \
./configure --prefix="$HOME/ffmpeg_build" --bindir="$HOME/bin" --disable-shared && \
make && \ 
make install
```

### 5. Install FFmpeg

> The third-party libraries selected by the following enable need to be compiled and installed. Some libraries are not specifically described above but the installation method is similar.

```
cd ~/ffmpeg_sources && \
wget -O ffmpeg-snapshot.tar.bz2 https://ffmpeg.org/releases/ffmpeg-snapshot.tar.bz2 && \
tar xjvf ffmpeg-snapshot.tar.bz2 && \
cd ffmpeg && \
PATH="$HOME/bin:$PATH" PKG_CONFIG_PATH="$HOME/ffmpeg_build/lib/pkgconfig" ./configure \
  --prefix="$HOME/ffmpeg_build" \
  --pkg-config-flags="--static" \
  --extra-cflags="-I$HOME/ffmpeg_build/include" \
  --extra-ldflags="-L$HOME/ffmpeg_build/lib" \
  --extra-libs="-lpthread -lm" \
  --ld="g++" \
  --bindir="$HOME/bin" \
  --enable-gpl \
  --enable-gnutls \
  --enable-libaom \
  --enable-libass \
  --enable-libfdk-aac \
  --enable-libfreetype \
  --enable-libmp3lame \
  --enable-libopus \
  --enable-libsvtav1 \
  --enable-libdav1d \
  --enable-libvorbis \
  --enable-libvpx \
  --enable-libx264 \
  --enable-libx265 \
  --enable-nonfree && \
PATH="$HOME/bin:$PATH" make && \
make install && \
hash -r
```

## 1.2: Using installed FFmpeg (Source Code Compilation)

1. move or copy binary file to `/usr/bin`

```
cp ~/bin/ffmpeg /usr/bin/
cp ~/bin/ffprobe /usr/bin/
```

> Cause the server does not have a graphical interface, FFPlay is not installed for the time being.

2. move or copy libraries and libs to your project

```
cp -r ~/ffmpeg_build/include <your project dictionary>
cp -r ~/ffmpeg_build/lib <your project dictionary>
```

3. Explicitly add statically linked libraries at compile time

Makefile Example

```
CC = g++
Cflag = -g
SRC = $(wildcard *.cpp)
OBJS = $(patsubst %cpp, %o, $(SRC))
HEADER = $(wildcard ../include/*.h)
Lib1 = -lavformat -lavdevice -lavcodec -lavutil -lpthread -lswscale -lswresample
Lib2 = -laom -lx264 -lx265 -lvpx -lfdk-aac -ldav1d -lopus -lmp3lame -lgnutls
Lib3 = -lz -lm -lbz2 -llzma
Lib4 = -ldl -lrt  
Path = <~/ffmpeg_build/lib>
INCLUDE = <~/ffmpeg_build/include>
Target = <your target name>

all: $(Target)

%.o:%.cpp $(HEADER)
	$(CC) $(Cflag) -c $< -o $@ -I $(INCLUDE)

$(Target):$(OBJS)
	$(CC) $(Cflag) -o $@ $^ -L $(Path) $(Lib1) $(Lib2) $(Lib3) $(Lib4)

.PHONY:clean
clean:
	rm -f *.o $(Target)
```




## 1.3: Install FFmpeg in Linux (Using Packaging Tool)

### 1 and 2

Step 1 and 2 refer to the practice in Section 1.1

### 3. Codec libraries

```
apt-get -y install nasm \
  libx264-dev \
  libx265-dev \ 
  libvpx-dev \
  libfdk-aac-dev \
  libaom-dev \
  libopus-dev \
  libmp3lame-dev
```

> Both packages need to be installed manually.

```
dpkg -i libdav1d4_0.7.1-3_amd64.deb
dpkg -i libdav1d-dev_0.7.1-3_amd64.deb
```

### 4. Install FFmpeg

```
wget -O ffmpeg-snapshot.tar.bz2 https://ffmpeg.org/releases/ffmpeg-snapshot.tar.bz2 && \
tar xjvf ffmpeg-snapshot.tar.bz2 && cd ffmpeg && \
./configure \
  --pkg-config-flags="--static" \
  --extra-cflags="-I/usr/include" \
  --extra-ldflags="-L/usr/lib/x86_64-linux-gnu" \
  --extra-libs="-lpthread -lm" \
  --ld="g++" \
  --enable-gpl \
  --enable-gnutls \
  --enable-libaom \
  --enable-libass \
  --enable-libfdk-aac \
  --enable-libfreetype \
  --enable-libmp3lame \
  --enable-libopus \
  --enable-libdav1d \
  --enable-libvpx \
  --enable-libx264 \
  --enable-libx265 \
  --enable-nonfree && \
make && \
make install && \
hash -r
```

## 1.4: Using installed FFmpeg (Using Packaging Tool)

MakeFile Example refer to Section 1.4
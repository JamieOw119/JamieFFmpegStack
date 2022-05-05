CC = g++
Cflag = -g
SRC = $(wildcard *.cpp)
OBJS = $(patsubst %cpp, %o, $(SRC))
HEADER = $(wildcard ../include/*.h)
Lib = -lavformat -lavdevice -lavcodec -lavutil -lpthread -lswscale -lswresample
Lib2 = -lz -lm -lbz2 -llzma
Lib3 = -ldl -lrt                                                         
Path = ~/ffmpeg-stack/lib
INCLUDE = ~/ffmpeg-stack/include
Target = VideoDecoding

all: $(Target)

%.o:%.cpp $(HEADER)
	$(CC) $(Cflag) -c $< -o $@ -I $(INCLUDE)

$(Target):$(OBJS)
	$(CC) $(Cflag) -o $@ $^ -L $(Path) $(Lib) ${Lib2} ${Lib3}

.PHONY:clean
clean:
	rm -f *.o $(Target)

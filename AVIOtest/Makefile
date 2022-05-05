CC = g++
Cflag = -g
SRC = $(wildcard *.cpp)
OBJS = $(patsubst %cpp, %o, $(SRC))
HEADER = $(wildcard ../include/*.h)
Lib = -lavformat -lavdevice -lavcodec -lavutil -lpthread -lswscale -lswresample                                                              
Path = ~/ffmpeg-stack/lib
INCLUDE = ~/ffmpeg-stack/include
Target = AVIOtest

all: $(Target)

%.o:%.cpp $(HEADER)
	$(CC) $(Cflag) -c $< -o $@ -I $(INCLUDE)

$(Target):$(OBJS)
	$(CC) $(Cflag) -o $@ $^ -L $(Path) $(Lib)

.PHONY:clean
clean:
	rm -f *.o $(Target)

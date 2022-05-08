CC = g++
Cflag = -g
SRC = $(wildcard *.cpp)
OBJS = $(patsubst %cpp, %o, $(SRC))
HEADER = $(wildcard *.h)
INCLUDE = ../include
INCOBJS = $(wildcard $(INCLUDE)/*.o)
Lib = -lavformat -lavdevice -lavcodec -lavutil -lswscale -lswresample                                                              
Lib2 = -laom -lx264 -lx265 -lvpx -lfdk-aac -lopus -lmp3lame -ldav1d
Lib3 = -lz -lm -lbz2 -llzma -ldl -lrt -lgnutls -lpthread
Target = VideoEncoding

all: $(Target)

%.o:%.cpp $(HEADER)
	$(CC) $(Cflag) -c $< -o $@ -I $(INCLUDE)

$(Target):$(OBJS) $(INCOBJS)
	$(CC) $(Cflag) -o $@ $^ $(Lib) $(Lib2) $(Lib3)

.PHONY:clean
clean:
	rm -f *.o $(Target)

CC = g++
Cflag = -g
SRC = $(wildcard *.cpp)
OBJS = $(patsubst %cpp, %o, $(SRC))
HEADER = $(wildcard ../include/*.h)
Lib = -lavformat -lavdevice -lavcodec -lavutil -lswscale -lswresample                                                              
Lib2 = -laom -lx264 -lx265 -lvpx -lfdk-aac -lopus -lmp3lame -ldav1d
Lib3 = -lz -lm -lbz2 -llzma -ldl -lrt -lgnutls -lpthread
Target = AVIOtest

all: $(Target)

%.o:%.cpp $(HEADER)
	$(CC) $(Cflag) -c $< -o $@

$(Target):$(OBJS)
	$(CC) $(Cflag) -o $@ $^ $(Lib) $(Lib2) $(Lib3)

.PHONY:clean
clean:
	rm -f *.o $(Target)

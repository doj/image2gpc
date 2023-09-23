CXXFLAGS += -Wall -std=c++17

all:	image2gpc

test:	all
	./image2gpc gtav-bw.png

clean:
	$(RM) image2gpc *.o *~

image2gpc:	image2gpc.o
	$(CXX) -o $@ $^

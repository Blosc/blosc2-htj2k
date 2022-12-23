.PHONY: build clean test

ROOT = /home/jdavid/sandboxes/Francesc/OpenHTJ2K
BLD = $(ROOT)/build
SRC = $(ROOT)/source

#CC = clang
#CXX = clang++
CC = gcc
CXX = g++

build:
	$(CXX) -c -o blosc2_htj2k.o blosc2_htj2k.cpp -I$(SRC)/core/interface -I$(SRC)/apps/decoder -fPIC
	$(CXX) -shared -o libblosc2_htj2k.so blosc2_htj2k.o\
		-L$(BLD)/bin -lopen_htj2k_R -Wl,-rpath $(BLD)/bin
	$(CC) main.c -L. -lblosc2_htj2k -Wl,-rpath .

clean:
	rm output/* -f
	rm a.out -f
	rm blosc2_htj2k.o libblosc2_htj2k.so -f

test: clean build
	./a.out

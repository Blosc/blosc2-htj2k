.PHONY: build clean test

BLOSC = /home/jdavid/sandboxes/Francesc/c-blosc2
CATERVA = /home/jdavid/sandboxes/Francesc/Caterva
HTJ2K = /home/jdavid/sandboxes/Francesc/OpenHTJ2K

#CC = clang
#CXX = clang++
CC = gcc
CXX = g++

build:
	$(CXX) -c -o blosc2_htj2k.o blosc2_htj2k.cpp -fPIC\
		-I$(BLOSC)/include\
		-I$(CATERVA)/caterva\
		-I$(HTJ2K)/source/core/interface -I$(HTJ2K)/source/apps/decoder
	$(CXX) -shared -o libblosc2_htj2k.so blosc2_htj2k.o\
		-L$(BLOSC)/build/blosc -lblosc2\
		-L$(CATERVA)/build -lcaterva\
		-L$(HTJ2K)/build/bin -lopen_htj2k_R\
		-Wl,-rpath $(HTJ2K)/build/bin
	$(CC) main.c\
		-I$(BLOSC)/include\
		-I$(CATERVA)/caterva\
		-L$(BLOSC)/build/blosc -lblosc2\
		-L$(CATERVA)/build -lcaterva\
		-L. -lblosc2_htj2k\
		-Wl,-rpath .

clean:
	rm a.out output -rf
	rm blosc2_htj2k.o libblosc2_htj2k.so -f

test: clean build
	mkdir output
	LD_LIBRARY_PATH="$(BLOSC)/build/blosc:$(CATERVA)/build" ./a.out

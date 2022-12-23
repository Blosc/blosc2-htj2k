.PHONY: build test

ROOT = /home/jdavid/sandboxes/Francesc/OpenHTJ2K

#CC = clang
#CXX = clang++
CC = gcc
CXX = g++

build:
	$(CXX) -c -o blosc2_htj2k.o blosc2_htj2k.cpp -I$(ROOT)/source/core/interface -fPIC -Wall
	$(CXX) -shared -o libblosc2_htj2k.so blosc2_htj2k.o\
		-L$(ROOT)/build/bin -lopen_htj2k_R\
		-Wl,-rpath $(ROOT)/build/bin
	$(CC) main.c -L. -lblosc2_htj2k -Wl,-rpath .

test: build
	rm output/* -f
	./a.out

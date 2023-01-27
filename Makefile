.PHONY: build clean openhtj2k submodules test

HTJ2K = contrib/OpenHTJ2K

CC = clang
CXX = clang++ --std=c++17
#CC = gcc
#CXX = g++

build: openhtj2k
	$(CXX) -c -o blosc2_htj2k.o blosc2_htj2k.cpp -fPIC\
		-I$(BLOSC)/include\
		-I$(BLOSC)\
		-I$(HTJ2K)/source/core/interface\
		-I$(HTJ2K)/source/apps/decoder
	$(CXX) -shared -o libblosc2_htj2k.so blosc2_htj2k.o\
		-L$(BLOSC)/build/blosc -lblosc2\
		-L$(HTJ2K)/build/bin -lopen_htj2k_R\
		-Wl,-rpath $(HTJ2K)/build/bin
	$(CC) main.c\
		-I$(BLOSC)/include\
		-I$(BLOSC)\
		-L$(BLOSC)/build/blosc -lblosc2\
		-L. -lblosc2_htj2k\
		-Wl,-rpath .

submodules:
	git submodule update --init

openhtj2k: submodules
	cd $(HTJ2K) && cmake -G "Unix Makefiles" -B build -DCMAKE_BUILD_TYPE=Release
	make -C $(HTJ2K)/build
	mkdir -p bin
	cp $(HTJ2K)/build/bin/* bin/

clean:
	rm a.out bin output -rf
	rm blosc2_htj2k.o libblosc2_htj2k.so -f

test: clean build
	mkdir output
	LD_LIBRARY_PATH="$(BLOSC)/build/blosc" ./a.out

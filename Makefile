all: build

build:
	clear && g++ -o ../bin/print_image print_image.cpp -O3 args.hpp

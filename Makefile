INSTALL_PATH?=/usr/local
CC=g++
CFLAGS= -Wall -Wextra -Wshadow -pedantic -std=c++14 -lpthread

all: build

clean:
	rm -f ../bin/*

build:
	$(CC) -o ../bin/print_image print_image.cpp args.hpp $(CFLAGS)

install:
	cp ../bin/print_image $(INSTALL_PATH)/bin/print_image

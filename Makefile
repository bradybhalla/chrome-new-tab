CC=emcc

FLAGS=--use-port=sdl2 -I include -gsource-map -O3

SRC=$(wildcard src/*.c)
STATIC_DIST_FILES=$(wildcard static/*)

.PHONY: all
all: dist compile_flags.txt

dist: build
	- mkdir dist
	- cp -r build/ dist/build/
	- cp $(STATIC_DIST_FILES) dist/

build: $(SRC)
	- mkdir build
	$(CC) $(FLAGS) -o build/main.js $(SRC)

compile_flags.txt:
	$(CC) $(FLAGS) --cflags | sed 's/ /\n/g' > compile_flags.txt

.PHONY: clean
clean:
	- rm -rf build
	- rm -rf dist
	- rm compile_flags.txt

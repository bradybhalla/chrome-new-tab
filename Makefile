CC=emcc

FLAGS=--use-port=sdl2

OUTPUT_FILE=build/main.js
SRC=$(wildcard src/*.cpp)

.PHONY: all
all: build compile_flags.txt

.PHONY: build
build: $(SRC)
	@mkdir -p $(shell dirname $(OUTPUT_FILE))
	$(CC) $(FLAGS) -o $(OUTPUT_FILE) $(SRC)

compile_flags.txt:
	$(CC) $(FLAGS) --cflags | sed 's/ /\n/g' > compile_flags.txt

.PHONY: clean_build
clean_build:
	- rm -rf build

.PHONY: clean
clean: clean_build
	- rm compile_flags.txt

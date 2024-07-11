CC=emcc

FLAGS=-s USE_SDL=2
DEBUG_FLAGS=-fsanitize=address -gsource-map
RELEASE_FLAGS=#-O2

OUTPUT=build/main.js
SRC=$(wildcard src/*)

$(OUTPUT): $(SRC)
	@mkdir -p $(shell dirname $(OUTPUT))
	$(CC) $(FLAGS) -o $(OUTPUT) $(SRC)

debug: FLAGS += $(DEBUG_FLAGS)
debug: $(OUTPUT)

release: FLAGS += $(RELEASE_FLAGS)
release: $(OUTPUT)

compile_flags.txt:
	$(CC) $(FLAGS) --cflags | sed 's/ /\n/g' > compile_flags.txt

clean:
	rm -rf build

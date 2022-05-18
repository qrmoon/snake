CC = gcc

INCLUDE_PATHS = -IC:\SDL\SDL2\include
LIBRARY_PATHS = -LC:\SDL\SDL2\lib\x86

CFLAGS = -Wall -Wextra -Wpedantic
LFLAGS = -lSDL2

ifeq ($(OS), Windows_NT)
	# -Wl,-subsystem,windows gets rid of the console window
	windows: CFLAGS += -Wl,-subsystem,windows
	windows: CFLAGS += -IC:\SDL\SDL2\include
	windows: LFLAGS += -lmingw32 -lSDL2main -lSDL2
	windows: LFLAGS += -LC:\SDL\SDL2\lib\x86
endif

BIN = bin/main

$(BIN): main.c
	$(CC) $^ $(CFLAGS) $(LFLAGS) -o $@

run: $(BIN)
	./$(BIN)

clean:
	rm $(BIN)*

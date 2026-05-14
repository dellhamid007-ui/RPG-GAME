CC = x86_64-w64-mingw32-gcc

CFLAGS = -Wall -Wextra -g \
         -Isrc \
         -Ilibs/raylib/include

LDFLAGS = -Llibs/raylib/lib
LDLIBS  = -lraylib -lopengl32 -lgdi32 -lwinmm

SRC = $(wildcard src/*.c) \
      $(wildcard src/components/*.c) \
      $(wildcard src/core/*.c) \
      $(wildcard src/systems/*.c) \
      $(wildcard src/ui/*.c) \
      $(wildcard src/misc/*.c)

TARGET = bin/game.exe

all: $(TARGET)

$(TARGET): $(SRC)
	@mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) $(LDLIBS) -o $(TARGET)

clean:
	rm -f $(TARGET)

CC = C:/mingw64/bin/gcc.exe
MAKE = C:/mingw64/bin/make.exe

CFLAGS = -Wall -Wextra -g \
         -Isrc \
         -Ilibs/raylib/include

LDFLAGS = -Llibs/raylib/lib
LDLIBS  = -lraylib -lopengl32 -lgdi32 -lwinmm

SRC = \
	src/main.c \
	src/components/enemy.c \
	src/components/items.c \
	src/components/player.c \
	src/components/world.c \
	src/core/entity.c \
	src/core/game.c \
	src/systems/combat.c \
	src/systems/dialogue.c \
	src/systems/quest.c \
	src/systems/save.c \
	src/ui/hud.c \
	src/ui/menus.c \
	src/ui/render.c \
	src/ui/text.c

TARGET = bin/game.exe

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) $(LDLIBS) -o $(TARGET)

clean:
	del bin\game.exe

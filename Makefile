CC     = gcc
CFLAGS = -Wextra -Wall -O2 -Iinclude -Wno-unused-parameter  -Wno-unused-function -Wno-unused-variable
LIBS   = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
TARGET = app

# FIX: Use 'find' to robustly gather all .c files inside 'src' and its subdirectories
SRC    = $(shell find src -name "*.c")

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(LIBS) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
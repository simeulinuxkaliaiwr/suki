CC = gcc
CFLAGS = -Wall -Wshadow -Wextra
SDLFLAGS = $(shell pkg-config --cflags --libs sdl2 SDL2_image SDL2_ttf fontconfig)
SRC = src/suki.c
TARGET = src/suki

all: $(TARGET)
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(SDLFLAGS)
clean:
	@rm -f $(TARGET)
rebuild: clean $(TARGET)
.PHONY: all clean rebuild

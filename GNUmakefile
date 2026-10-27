CC = gcc
CFLAGS = -Wall -Wshadow -Wextra $(shell pkg-config --cflags --libs sdl2 SDL2_image SDL2_ttf)
SRC = src/suki.c
TARGET = src/suki

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)
clean:
	@rm -f $(TARGET)

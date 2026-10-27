CC = gcc
CFLAGS = -Wall -Wshadow -Wextra
SDLFLAGS = $(shell pkg-config --cflags --libs sdl2 SDL2_image SDL2_ttf)
SRC = src/suki.c
TARGET = src/suki

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(SDLFLAGS)
rebuild:
	@rm -f $(TARGET)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(SDLFLAGS)
clean:
	@rm -f $(TARGET)

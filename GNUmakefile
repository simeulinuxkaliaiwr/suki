CC = gcc
CFLAGS = -I/usr/include/SDL2 -DHWY_SHARED_DEFINE -DAVIF_DLL -I/usr/include/rav1e -I/usr/include/svt-av1 -DEB_DLL -DRTC_BUILD=0 -I/usr/include/webp -D_GNU_SOURCE=1 -D_REENTRANT -I/usr/include/harfbuzz -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/sysprof-6 -pthread -I/usr/include/freetype2 -I/usr/include/libpng16 -lSDL2_image -lSDL2_ttf -lSDL2 
SRC = src/suki.c
TARGET = src/suki

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)
rebuild:
	@rm -f $(TARGET)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)
clean:
	@rm -f $(TARGET)

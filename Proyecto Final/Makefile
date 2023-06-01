CC = g++
CFLAGS = -g -I/usr/include/SDL2 -std=c++11
DEPS = Vector2.h SDLUtils.h SDLTexture.h Socket.h Casilla.h Message.h Player.h Server.h
OBJ = Vector2.o SDLUtils.o  SDLTexture.o Socket.o Casilla.o Message.o Player.o Server.o
LIBS = -lpthread -lSDL2 -lSDL2_mixer -lSDL2_ttf -lSDL2_image

%.o: %.cc $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

all: Player Server

Player: $(OBJ) InitPlayer.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

Server: $(OBJ) InitServer.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o Player Server
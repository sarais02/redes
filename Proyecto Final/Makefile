CC = g++
CFLAGS = -g -I/usr/include/SDL2 -std=c++11
DEPS = Socket.h Vector2.h Casilla.h
OBJ = Socket.o Vector2.o Casilla.o
LIBS = -lpthread -lSDL2 -lSDL2_mixer -lSDL2_ttf

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
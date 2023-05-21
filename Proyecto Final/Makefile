CC = g++
CFLAGS = -g -I/usr/include/SDL2 -std=c++11
DEPS = Socket.h #Chat.h
OBJ = Socket.o #Chat.o
LIBS = -lpthread -lSDL2 -lSDL2_mixer -lSDL2_ttf

%.o: %.cc $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

all: Player Server

Player: $(OBJ) Player.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

Server: $(OBJ) Server.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o Player Server
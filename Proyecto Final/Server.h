#ifndef SERVER_H_
#define SERVER_H_

#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <memory>

#include "Socket.h"
#include "Casilla.h"
#include "Player.h"
#include "Message.h"

class Server{
public:
    Server(const char * s, const char * p): socket(s, p){
        socket.bind();
    };
    void init();
    /**
     *  Thread principal del servidor recive mensajes en el socket y
     *  lo distribuye a los clientes. Mantiene actualizada la lista de clientes
     */
    void do_messages();
    void input_thread();
private:
    /**
     *  Lista de clientes conectados al servidor, representados por
     *  su socket
     */
    std::vector<std::unique_ptr<Socket>> socketsPlayers;
    /**
     * Tablero de Juego
     * 
    */
    std::vector<Casilla*> tablero;
    /**
     * TJugadores
     * 
    */
    std::vector<PlayerSerializable> players;
    /**
     * Socket del servidor
     */
    Socket socket;
    /**
     * vector con los indices de los jugadores por turnos
     */
    std::vector<int> turnos;
    int indexTurno=0;
    /**
     * booleano para ver el estado de la partida 
     */
    bool onGame=false;
    /**
     * Inicializa las casillas del tablero 
    */
    void initTablero();
    /**
    * Sincroniza todos los players de cara a empezar la partida y Randomizar los turnos
    */
    void initPlayers();
};
#endif
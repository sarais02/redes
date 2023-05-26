#ifndef SERVER_H_
#define SERVER_H_

#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <memory>


#include "Serializable.h"
#include "Socket.h"
#include "Casilla.h"
#include "Player.h"
#include "ChatMessage.h"


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
    std::vector<std::unique_ptr<Casilla>> tablero;
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
     * Guardar los turnos la clave es el turno y el valor el identificador del jugador
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
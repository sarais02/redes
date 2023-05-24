#pragma once
#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <memory>

#include "Serializable.h"
#include "Socket.h"
#include "Casilla.h"

class Server
{
public:
    Server(const char * s, const char * p): socket(s, p){
        socket.bind();
    };

    /**
     *  Thread principal del servidor recive mensajes en el socket y
     *  lo distribuye a los clientes. Mantiene actualizada la lista de clientes
     */
    void do_messages();

private:
    /**
     *  Lista de clientes conectados al servidor, representados por
     *  su socket
     */
    std::vector<std::unique_ptr<Socket>> players;
    /**
     * Tablero de Juego
     * 
    */
    Casilla tablero[40];
    /**
     * Socket del servidor
     */
    Socket socket;
    /**
     * Inicializa las casillas del tablero
    */
    void initTablero();
};
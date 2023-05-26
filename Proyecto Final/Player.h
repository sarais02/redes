#ifndef PLAYER_H_
#define PLAYER_H_

#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <memory>

#include "Serializable.h"
#include "Socket.h"
#include "Casilla.h"


/**
 *  Clase para el cliente de chat
 */
class Player{
public:
    /**
     * @param s dirección del servidor
     * @param p puerto del servidor
     * @param n nick del usuario
     */
    Player(const char * s, const char * p, const char * n):socket(s, p),
        nick(n){};

    /**
     *  Envía el mensaje de login al servidor
     */
    void login();

    /**
     *  Envía el mensaje de logout al servidor
     */
    void logout();

    /**
     *  Rutina principal para el Thread de E/S. Lee datos de STDIN (std::getline)
     *  y los envía por red vía el Socket.
     */
    void input_thread();

    /**
     *  Rutina del thread de Red. Recibe datos de la red y los "renderiza"
     *  en STDOUT
     */
    void net_thread();


    void to_bin();

    int from_bin(char * bobj);

private:

    /**
     * Socket para comunicar con el servidor
     */
    Socket socket;

    bool isMyTurn;

    /**
     * Nick del usuario
     */
    std::string nick;
    int indexPosition;
    int indexPlayer=-1;
    int dinero;
};
#endif
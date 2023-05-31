#ifndef PLAYER_H_
#define PLAYER_H_

#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <memory>
#include <unordered_map>

#include "Casilla.h"
#include "Serializable.h"
#include "Socket.h"
#include "Message.h"

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
        nick(n){playerProperties=std::unordered_map<int,std::string>();};

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
    /**
     * Booleano que representa si es el turno del jugador
     */
    bool isMyTurn = false;
     /**
     * Booleano que representa si el jugador puede terminar su turno
     */
    bool canFinishMyTurn = false;
    /**
     * Booleano que representa si el jugador puede comprar una casilla
     */
    bool canBuySomething = false;
    /**
     * Lo que me puedo Comprar
     */
    ComprarCalleMsg compra;
    /**
     * Booleano que representa si el jugador debe pagar a alguien
     */
    bool needToPay;
    /**
     * Nick del usuario
     */
    std::string nick;
    /**
     * Representa la posicion del jugador en el array de casillas
     */
    int indexPosition;
    /**
     * Representa la posicion del player en el array de jugadores del servidor
     */
    int indexPlayer=-1;
    /**
     * Dinero del jugador
     */
    int money;
    /**
     * Contador de turnos en la carcel
     */
    int cont=0;
    //
    bool isInJail=false;
    int moneyToPay;
    std::unordered_map<int,std::string> playerProperties;
};
#endif
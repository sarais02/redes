#ifndef SERVER_H_
#define SERVER_H_

#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <memory>
#include <list>

#include "Socket.h"
#include "Casilla.h"
#include "Player.h"
#include "Message.h"

class Server{
public:
    Server(const char * s, const char * p): socket(s, p){
        socket.bind();
    };
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
     * booleano para ver si el jugador ya ha tirado los dados
     */
    bool canFinishTurn=false;
    
    /**
     * Inicializa las casillas del tablero 
    */
    void initTablero();
    /**
    * Sincroniza todos los players de cara a empezar la partida y Randomizar los turnos
    */
    void initPlayers();
    /**
    * Consequencias del movimiento del jugador
    */
    void movementConsequences(int indexPlayer);
    /**
    * actualizar el estado del tablero al comprar una casilla
    */
    void comprarCasilla();
    /**
    * Poner Casa
    */
    bool tryPonerCasas(int indexPosition,int numCasas);
    bool tengoColor(int indexPosition);
    void initCartasSuerte();
    void initCartasComunidad();
    //CARTAS DE SUERTE
    std::list<std::pair<std::string, int>> suerte_pagar;
    std::list<std::pair<std::string, int>> suerte_cobrar;
    std::list<std::pair<std::string, int>> suerte_avanzar;
    //CARTAS DE COMUNIDAD
    std::list<std::pair<std::string, int>> comunidad_pagar;
    std::list<std::pair<std::string, int>> comunidad_cobrar;
    std::list<std::pair<std::string, int>> comunidad_avanzar;
};
#endif
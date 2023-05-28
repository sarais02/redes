#include "Server.h"
#include <random>
#include "Message.h"
void Server::init(){
    initTablero();
}
void Server::do_messages(){
   while (true){
        Message tmpMessage;      
        //Recibir Mensajes en y en socketfunción del tipo de mensaje
        Socket* socket_cliente;
        socket.recv(tmpMessage, socket_cliente);                      
          
        // - LOGIN: Añadir al vectosocketr clients
        // - LOGOUT: Eliminar del vector clients
        // - MESSAGE: Reenviar el mensaje a todos los clientes (menos el emisor)
        switch(tmpMessage.getType()){
            case LOGIN:{
                IN_OUT in;
                in.from_bin(tmpMessage.data());
                std::unique_ptr<Socket>socket1_(socket_cliente);
                socketsPlayers.push_back(std::move(socket1_));
                std::cout<<"LOGIN DE: "<<in.nick<<"\n";

                PlayerSerializable tmpPLayer(in.nick, 0, 1500,(int)(socketsPlayers.size()-1));
                players.push_back(tmpPLayer);    

                break;
            }  
            case LOGOUT:{
                IN_OUT out;
                out.from_bin(tmpMessage.data());
                std::unique_ptr<Socket>socket_(socket_cliente);
                for(auto it=socketsPlayers.begin();it!=socketsPlayers.end();){
                    if(*(*it)==*socket_cliente){
                        socketsPlayers.erase(it);
                    }
                    else ++it;
                }
                std::cout<<"LOGOUT DE: "<<out.nick<<"\n";
                break;
            }
            case PLAYERSERIALIZABLE:{
                PlayerSerializable player;
                player.from_bin(tmpMessage.data());
                players[player.indexPlayer]=player;
                std::cout<<"Player se ha movido: "<<player.indexPosition<<"\n";
                break;
            }
            case ENDTURN:{
                indexTurno =indexTurno+1 % turnos.size();               
                Message initturno;
                initturno.setType(INITURN);
                socket.send(initturno, *socketsPlayers[turnos[indexTurno]]);
            }
        }
        //}
        //else if(tmp.size()==PlayerSerializable::MESSAGE_SIZE){
        //    PlayerSerializable player;
        //    player.from_bin(tmp.data());
        //}
    }
}

void Server::input_thread(){
    while(true){
        std::string msg;
        std::getline(std::cin,msg);      
        if(msg=="start"&&!onGame){
            
            initTablero();
            initPlayers();
        }
    }
}
void Server::initTablero(){
    tablero = std::vector<Casilla*>(40);
    for(int i=0; i<40;i++){
        tablero[i] = new Casilla();
    }
}
void Server::initPlayers(){
    onGame=true;
    int max=socketsPlayers.size();
    turnos = std::vector<int>(max,-1);
    for (int i = 0; i < max; i++){
        //ASIGNACION TURNOS
        int rand= std::rand() % max;
        while(turnos[rand]!=-1)rand= std::rand() % max;
        turnos[rand]=i;
    }
    int i=0;
    for(auto it=socketsPlayers.begin(); it!=socketsPlayers.end(); ++it){
        if((*it)==nullptr)std::cout<<"NULL\n";
        socket.send(players[i], *(*it));
        std::cout<<"ENVIADO "<<players[i].indexPlayer<<"\n";
        i++;
    }
    Message initturno;
    initturno.setType(INITURN);
    socket.send(initturno, *socketsPlayers[turnos[indexTurno]]);
    std::cout<<"ENVIADO TURNO A: "<<turnos[indexTurno]<<"\n";
}
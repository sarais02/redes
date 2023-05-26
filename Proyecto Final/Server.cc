#include "Server.h"
#include "ChatMessage.h"
#include <random>

void Server::init(){
    initTablero();
}
void Server::do_messages(){
   while (true)
    {

        /*
         * NOTA: los clientes están definidos con "smart pointers", es necesario
         * crear un unique_ptr con el objeto socket recibido y usar std::move
         * para añadirlo al vector
         */
        
        ChatMessage mensaje;
        Serializable tmp;
        //Recibir Mensajes en y en socketfunción del tipo de mensaje
        Socket* socket_cliente= new Socket(socket);
        socket.recv(mensaje, socket_cliente);
        //if(tmp.size()==ChatMessage::MESSAGE_SIZE){
           
            
            //mensaje.from_bin(tmp.data());
            // - LOGIN: Añadir al vectosocketr clients
            // - LOGOUT: Eliminar del vector clients
            // - MESSAGE: Reenviar el mensaje a todos los clientes (menos el emisor)
            switch(mensaje.type){
                case ChatMessage::LOGIN:{
                    std::unique_ptr<Socket>socket1_(socket_cliente);
                    socketsPlayers.push_back(std::move(socket1_));
                    std::cout<<"LOGIN DE: "<<mensaje.nick<<"\n";

                    PlayerSerializable tmpPLayer(mensaje.nick, 0, 1500,(int)(socketsPlayers.size()-1));
                    players.push_back(tmpPLayer);    

                    //PlayerSerializable tmpPLayer(mensaje.nick, 0, 1500,(int)(socketsPlayers.size()-1));
                    //players.push_back(tmpPLayer);
                    
                    //int r =socket.send(tmpPLayer, *(socket1_.get()));
                    //std::cout<<"ENVIADO\n";
                    //if(r==-1){
                    //    std::cout<<"ERROR en el send"<<"\n";
                    //}
                    break;
                }  
                case ChatMessage::LOGOUT:{
                    std::unique_ptr<Socket>socket_(socket_cliente);
                    for(auto it=socketsPlayers.begin();it!=socketsPlayers.end();){
                        if(*(*it)==*socket_cliente){
                            socketsPlayers.erase(it);
                        }
                        else ++it;
                    }
                    std::cout<<"LOGOUT DE: "<<mensaje.nick<<"\n";
                    break;
                }
                
                case ChatMessage::MESSAGE:{
                    std::cout<<"mandando mensaje"<<std::endl;
                    //std::unique_ptr<Socket>socket_(socket_cliente);
                    for(auto it=socketsPlayers.begin();it!=socketsPlayers.end();){
                        if(*(*it)==*socket_cliente){
                            std::cout<<"no soy yo"<<std::endl;
                        }
                        else{
                            socket.send(mensaje, *(*it));
                            std::cout<<"enviando"<<std::endl;
                        } 
                    ++it;
                    }
                    break;
                }
                case ChatMessage::THROW:{
                    std::cout<<"mandando mensaje"<<std::endl;
                    //std::unique_ptr<Socket>socket_(socket_cliente);
                    for(auto it=socketsPlayers.begin();it!=socketsPlayers.end();){
                        if(*(*it)==*socket_cliente){
                            std::cout<<"no soy yo"<<std::endl;
                        }
                        else{
                            socket.send(mensaje, *(*it));
                            std::cout<<"enviando"<<std::endl;
                        } 
                    ++it;
                    }
                    break;
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
            break;
        }
    }
}
void Server::initTablero(){
    tablero = std::vector<std::unique_ptr<Casilla>>(40);
    for(int i=0; i<40;i++){
        tablero[i] = std::unique_ptr<Casilla>();
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
    std::cout<<"ENTRA "<<players.size()<<"\n";
    int i=0;
    for(auto it=socketsPlayers.begin(); it!=socketsPlayers.end(); ++it){
        if((*it)==nullptr)std::cout<<"NULL\n";
        socket.send(players[i], *(*it));
        std::cout<<"ENVIADO "<<i<<"\n";
        i++;
    }
   
    indexTurno++;
}
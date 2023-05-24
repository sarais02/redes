#include "Server.h"

void Server::do_messages(){
   while (true)
    {
        /*
         * NOTA: los clientes están definidos con "smart pointers", es necesario
         * crear un unique_ptr con el objeto socket recibido y usar std::move
         * para añadirlo al vector
         */

        //Recibir Mensajes en y en función del tipo de mensaje
        // - LOGIN: Añadir al vector clients
        // - LOGOUT: Eliminar del vector clients
        // - MESSAGE: Reenviar el mensaje a todos los clientes (menos el emisor)
        //Socket* clientSd;
        //ChatMessage msg;
        //Recibir Mensajes en y en función del tipo de mensaje
        //socket.recv(msg, clientSd);

        //if(msg.type == ChatMessage::MessageType::LOGIN){
        //    std::cout << msg.nick << " LOGIN\n";
        //    clients.push_back(std::unique_ptr<Socket>(std::move(clientSd)));
        //}
        // - LOGOUT: Eliminar del vector clients
        //else if(msg.type == ChatMessage::MessageType::LOGOUT){
        //    std::cout << msg.nick << " LOGOUT\n";
        //    auto it = clients.begin();
        //    while(it != clients.end() && !(*(*it).get() == *clientSd)) it++;
        //    clients.erase(it);
        //}
        // - MESSAGE: Reenviar el mensaje a todos los clientes (menos el emisor)
        //else if(msg.type == ChatMessage::MessageType::MESSAGE){
        //    std::cout << msg.nick << " MESSAGE\n";
        //    for(int i=0; i<clients.size(); ++i){
        //        if(!(*(clients[i].get()) == *clientSd))
        //            socket.send(msg, (*clients[i].get()));
        //    }
        //}

    }
}
void Server::initTablero(){

}
#include "Player.h"
#include "ChatMessage.h"

void Player::login(){
    std::string msg;

    ChatMessage em(nick, msg);
    em.type = ChatMessage::LOGIN;

    socket.send(em, socket);
}

void Player::logout(){
    // Completar
    std::string msg;

    ChatMessage em(nick, msg);
    em.type = ChatMessage::LOGOUT;

    socket.send(em, socket);
}

void Player::input_thread(){
    while (true)
    {
        // Leer stdin con std::getline
        // Enviar al servidor usando socket
        std::string msg;
        std::getline(std::cin,msg);
        ChatMessage chatMSG(nick,msg);
        if(msg=="q"){
            logout();
            break;
        }
        if(isMyTurn){
            if(msg=="t"){
                
                int dado1= std::rand() %6 + 1;
                indexPosition+=dado1;
                int dado2= std::rand() %6 + 1;
                indexPosition+=dado2;
                indexPosition%=39;
                std::cout<<"Tiro: "<<dado1<<" "<<dado2<<"\n";
                chatMSG.type=ChatMessage::MessageType::THROW;
                PlayerSerializable player(nick,indexPosition,dinero,indexPlayer);
                socket.send(player,socket);
            }
        }
        else socket.send(chatMSG,socket);
    }
}

void Player::net_thread(){
    while(true)
    {
        //Recibir Mensajes de red
        Serializable mensaje;
        socket.recv(mensaje);

        if(mensaje.size()==ChatMessage::MESSAGE_SIZE){
            ChatMessage chatMSG;
            chatMSG.from_bin(mensaje.data());
            std::cout << chatMSG.nick << ": " << chatMSG.message << "\n";
        }
        else if(mensaje.size()==PlayerSerializable::MESSAGE_SIZE){
            PlayerSerializable playerMSG;
            playerMSG.from_bin(mensaje.data());
            if(playerMSG.indexPlayer==indexPlayer || indexPlayer==-1){
                indexPosition=playerMSG.indexPosition;
                indexPlayer=playerMSG.indexPlayer;
                dinero=playerMSG.dinero;
                std::cout<<"INIT RECIDIBIDO "<<indexPlayer<<"\n";
            }
        }
    }
}
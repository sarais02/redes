#include "Player.h"
#include "Message.h"

void Player::login(){
    std::string msg;

    IN_OUT em(nick, msg,true);   

    socket.send(em, socket);
}

void Player::logout(){
    // Completar
    std::string msg;

    IN_OUT em(nick, msg,false);

    socket.send(em, socket);
}

void Player::input_thread(){
    while (true)
    {
        // Leer stdin con std::getline
        // Enviar al servidor usando socket
        std::string msg;
        std::getline(std::cin,msg);        
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
                std::cout<<"IndexPosition: "<<indexPosition<<"\n";              
                PlayerSerializable player(nick,indexPosition,dinero,indexPlayer);
                socket.send(player,socket);
            }
            if(msg=="e"){
                std::cout<<"Acabo Mi Turno\n";
                Message initturno;
                initturno.setType(ENDTURN);
                socket.send(initturno, socket);
                isMyTurn=false;
            }
        }
        
    }
}

void Player::net_thread(){
    while(true)
    {
        //Recibir Mensajes de red
        Message tmpMessage;
        socket.recv(tmpMessage);

        switch (tmpMessage.getType()){
            case PLAYERSERIALIZABLE:{
                PlayerSerializable playerMSG;
                playerMSG.from_bin(tmpMessage.data());
                if(playerMSG.indexPlayer==indexPlayer || indexPlayer==-1){
                    indexPosition=playerMSG.indexPosition;
                    indexPlayer=playerMSG.indexPlayer;
                    dinero=playerMSG.dinero;
                    std::cout<<"INIT RECIDIBIDO "<<indexPlayer<<"\n";                   
                }
                break;
            }
            case INITURN:{
                std::cout<<"MI TURNO\n";
                isMyTurn=true;
            }          
        }        
    }
}
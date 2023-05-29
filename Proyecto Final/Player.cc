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
            if(msg=="t"){ //TIRAR DADOS                              
                int dado1= std::rand() %6 + 1;
                indexPosition+=dado1;
                int dado2= std::rand() %6 + 1;
                indexPosition+=dado2;
                indexPosition%=39;               
                std::cout<<"Tiro: "<<dado1<<" "<<dado2<<"\n";
                std::cout<<"IndexPosition: "<<indexPosition<<"\n";              
                PlayerSerializable player(nick,indexPosition,money,indexPlayer);//POR DEFECTO ES MOVER EL TIPO DE MENSAJE PLAYERSERIALIZABLE
                socket.send(player,socket);
            }
            if(msg=="e"&&canFinishMyTurn){ //SI PUEDO ACABAR TURNO
                std::cout<<"Acabo Mi Turno\n";
                Message initturno;
                initturno.setType(ENDTURN);
                socket.send(initturno, socket);
                isMyTurn=false;
            }
            if(msg=="e"&&!canFinishMyTurn)std::cout<<"No puedes acabar tu turno\n";

            if(msg=="c"&&canBuySomething&&compra.buyPrice<=money){ //SI PUEDO COMPRAR 
                compra.setType(COMPRADA);
                socket.send(compra,socket);                
                money-=compra.buyPrice;             
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
            case MOVER:{
                PlayerSerializable playerMSG;
                playerMSG.from_bin(tmpMessage.data());
                if(playerMSG.indexPlayer==indexPlayer || indexPlayer==-1){
                    indexPosition=playerMSG.indexPosition;
                    indexPlayer=playerMSG.indexPlayer;
                    money=playerMSG.dinero;
                    std::cout<<"INIT RECIDIBIDO "<<indexPlayer<<"\n";                   
                }
                break;
            }
            case INITURN:{
                std::cout<<"MI TURNO\n";
                isMyTurn=true;
                canFinishMyTurn=false;              
                break;
            }
            case CANENDTURN:{
                std::cout<<"PUEDO TERMINAR MI TURNO\n";
                canFinishMyTurn=true;
                break;
            } 
            case COMPRAR:{
                compra = ComprarCalleMsg();
                canBuySomething=true;
                compra.from_bin(tmpMessage.data());
                //GUARDARME LO K PUEDO COMPRAR
                std::cout<<"Quieres comprar "<<compra.nombre<<" por "<<compra.buyPrice<<"?";
                break;
            }        
        }        
    }
}
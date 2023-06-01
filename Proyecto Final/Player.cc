#include "Player.h"
#include <cstdlib>
#include <ctime>
#include "Message.h"
#include <sstream>
#include <thread>
void Player::login(){
    std::string msg;

    IN_OUT em(nick, msg,true);   

    socket.send(em, socket);
    loadWindow();
}

void Player::logout(){
    // Completar
    std::string msg;

    IN_OUT em(nick, msg,false);

    socket.send(em, socket);
}

void Player::loadWindow(){
    SDLUtils::init("Monopoly Online", 800, 600);

    texturaMapa=SDLTexture(SDLUtils::instance()->renderer(), "Images/Tablero_1.jpg",{20,0,760,560});      
    texturaJugador=SDLTexture(SDLUtils::instance()->renderer(), "Images/Suerte_1.jpg",{20,20,20,20});  
    texturaJugador.setPosition({693.5,520});
    
}

void Player::input_thread(){
    std::srand(std::time(0));
    while (!exit){       
        // Leer stdin con std::getline
        // Enviar al servidor usando socket
        std::string msg;
        std::getline(std::cin,msg);        
        if(msg=="q"){
            logout();
            break;
        }
        if(isMyTurn){
            if(msg=="t"&&!canFinishMyTurn){ //TIRAR DADOS                              
                int dado1= std::rand() %6 + 1;
                int dado2= std::rand() %6 + 1;
                int suma=dado1+dado2+indexPosition;
                suma%=40;
                if(suma<indexPosition && suma!=0) //Si suma justo es la salida no entra porq se sumaria 2 veces
                {
                    PlayerSerializable player(nick,0,money,indexPlayer);//Muevo al jugador a la salida
                    socket.send(player,socket);
                }
                indexPosition=suma;
                              
                std::cout<<"Tiro: "<<dado1<<" "<<dado2<<"\n";
                std::cout<<"IndexPosition: "<<indexPosition<<"\n";              
                PlayerSerializable player(nick,indexPosition,money,indexPlayer);//POR DEFECTO ES MOVER EL TIPO DE MENSAJE PLAYERSERIALIZABLE
                socket.send(player,socket);
                moverPlayer();
            }
            if(msg=="e"&&canFinishMyTurn){ //SI PUEDO ACABAR TURNO
                std::cout<<"Acabo Mi Turno\n";
                Message initturno;
                initturno.setType(ENDTURN);
                socket.send(initturno, socket);
                isMyTurn=false;
                canBuySomething=false;
            }
            if(msg=="e"&&!canFinishMyTurn)std::cout<<"No puedes acabar tu turno\n";

            if(msg=="c" && canBuySomething && compra.buyPrice<=money){ //SI PUEDO COMPRAR 
                canBuySomething=false;
                socket.send(compra,socket);                
                money-=compra.buyPrice;
                playerProperties.insert({compra.indexCasilla,compra.nombre});
                std::cout<<"Calle Comprada\n";                             
            }
            if(msg=="s" && isInJail && money>=moneyToPay){
                isInJail=false;
                money-=moneyToPay;
                std::cout<<"Fianza Pagada\n";
            }
            if(msg=="n" && isInJail){
                std::cout<<"Te quedas en la carcel\n";
                std::cout<<"Se acaba tu turno\n";
                Message initturno;
                initturno.setType(ENDTURN);
                socket.send(initturno, socket);
                isMyTurn=false;
                canBuySomething=false;
            }
            if(msg[0]=='p'){
                std::istringstream iss(msg);
                std::string index,numCasas,quitar;
                std::getline(iss,index,' ');
                std::getline(iss,index,' ');
                std::getline(iss,numCasas,' ');
                std::getline(iss,quitar,' ');
                if(std::isdigit(index[0])&&std::isdigit(numCasas[0])){ //SOLO CARACTERES NUMERICOS
                    
                    CasaMsg casa(std::stoi(index),std::stoi(numCasas));
                    if(quitar=="q")casa.quitarCasas=1;
                    socket.send(casa, socket);
                }
            }
            if(msg[0]=='m'){ //QUITAR
                std::istringstream iss(msg);
                std::string num;
                std::getline(iss,num,' ');
                std::getline(iss,num,' ');
                int suma=std::stoi(num);
                suma%=40;
                if(suma<indexPosition && suma!=0) //Si suma justo es la salida no entra porq se sumaria 2 veces
                {
                    PlayerSerializable player(nick,0,money,indexPlayer);//Muevo al jugador a la salida
                    socket.send(player,socket);
                }
                indexPosition=suma;

                std::cout<<"IndexPosition: "<<indexPosition<<"\n";              
                PlayerSerializable player(nick,indexPosition,money,indexPlayer);//POR DEFECTO ES MOVER EL TIPO DE MENSAJE PLAYERSERIALIZABLE
                socket.send(player,socket);
            }
            if(msg[0]=='h'){
                std::istringstream iss(msg);
                std::string num,hipotecar;
                std::getline(iss,num,' ');
                std::getline(iss,num,' ');
                std::getline(iss,hipotecar,' ');               
                int aux=hipotecar=="hipotecar"?1:0;              
                HipotecaMsg hipoteca(std::stoi(num),aux);
                socket.send(hipoteca,socket);
            }
        }        
    }
    std::cout<<"termina\n";
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
                if(isInJail){
                    cont++;
                    if(cont>3) //Si ya lleva 3 turnos en la carcel sale
                    {
                        cont=0;
                        isMyTurn=true;
                        canFinishMyTurn=false;
                    }
                    else //Si no acabo mi turno
                    {
                        isMyTurn=false;
                        std::cout<<"Acabo Mi Turno\n";
                        Message initturno;
                        initturno.setType(ENDTURN);
                        socket.send(initturno, socket);
                        canBuySomething=false;
                    }
                }
                else{
                    isMyTurn=true;
                    canFinishMyTurn=false;  
                }         
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
                std::cout<<compra.indexCasilla<<"\n";
                std::cout<<"Quieres comprar "<<compra.nombre<<" por "<<compra.buyPrice<<"? PRESS C TO BUY \n";
                break;
            } 
            case PAGAR:{
                PagarMsg pagar;
                pagar.from_bin(tmpMessage.data());
                money-=pagar.buyPrice;
                std::cout<<"Pagas "<<pagar.buyPrice<<"\n";
                break;
            }
            case COBRAR:{
                PagarMsg cobra;
                cobra.from_bin(tmpMessage.data());
                money+=cobra.buyPrice;
                std::cout<<"Cobras "<<cobra.buyPrice<<"\n ";
                break;
            }
            case IRACARCEL:{
                isInJail=true;
                cont=0;
                CarcelMsg carcel;
                carcel.from_bin(tmpMessage.data());
                moneyToPay=carcel.buyPrice;
                std::cout<<"Quieres pagar "<<moneyToPay<<
                " para salir de la carcel? PRESS S TO ACCEPT OR N TO REJECT \n";
                break;
            }
            case CASA:{
                CasaMsg casa;
                casa.from_bin(tmpMessage.data());
                std::cout<<"SOLICITUD DE CASA: "<<casa.msgResponse;
                break;
            }
            case HIPOTECA:{
                HipotecaMsg hipoteca;
                hipoteca.from_bin(tmpMessage.data());
                std::cout<<"SOLICITUD DE HIPOTECAR: "<<hipoteca.msgResponse<<"\n";
                break;
            }
        }        
    }
}
void Player::bucleVentana(){
    std::cout<<"Hola hilo\n";
    while(!exit){
        Uint32 startTime = sdlutils().currRealTime();
        gestionEventos();
        //RENDERIZAR IMAGENES       
        sdlutils().clearRenderer({0, 0, 0});      

        texturaMapa.render();
        texturaJugador.render();

        sdlutils().presentRenderer();//TE PRESENTA LOS CAMBIOS EN LA VENTANA
        Uint32 frameTime = sdlutils().currRealTime() - startTime;
		if (frameTime < 20)
			SDL_Delay(20 - frameTime);
    }
}
void Player::gestionEventos(){
    SDL_Event event;
    while (SDL_PollEvent(&event)){
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)){
            exit=true;
            std::cout<<"CERRAR VENTANA\n";
            sdlutils().closeWindow();
            logout();              
        }
        if(event.type == SDL_KEYDOWN){
            switch (event.key.keysym.scancode){
                case SDL_SCANCODE_SPACE:{ //TIRAR LOS DADOS
                    if(!canFinishMyTurn){ //TIRAR DADOS                              
                        int dado1= 0;//std::rand() %6 + 1;
                        int dado2= 3;//std::rand() %6 + 1;
                        int suma=dado1+dado2+indexPosition;
                        suma%=40;
                        if(suma<indexPosition && suma!=0) //Si suma justo es la salida no entra porq se sumaria 2 veces
                        {
                            PlayerSerializable player(nick,0,money,indexPlayer);//Muevo al jugador a la salida
                            socket.send(player,socket);
                        }
                        indexPosition=suma;
                            
                        std::cout<<"Tiro: "<<dado1<<" "<<dado2<<"\n";
                        std::cout<<"IndexPosition: "<<indexPosition<<"\n";              
                        PlayerSerializable player(nick,indexPosition,money,indexPlayer);//POR DEFECTO ES MOVER EL TIPO DE MENSAJE PLAYERSERIALIZABLE
                        socket.send(player,socket);
                        moverPlayer();
                    }
                    break;
                }
                case SDL_SCANCODE_E:{
                    if(canFinishMyTurn){ //SI PUEDO ACABAR TURNO
                        std::cout<<"Acabo Mi Turno\n";
                        Message initturno;
                        initturno.setType(ENDTURN);
                        socket.send(initturno, socket);
                        isMyTurn=false;
                        canBuySomething=false;
                    }       
                    break;
                }
            default:
                break;
            }
        }
    }
}
void Player::moverPlayer(){

    if(indexPosition>30){
       Vector2 pos= texturaJugador.getPosition();
       pos={720,(double)(90+45*(indexPosition%30))};
       texturaJugador.setPosition(pos);
       std::cout<<pos.getX()<<" "<<pos.getY()<<"\n";
    }
    else if(indexPosition==30){
    Vector2 pos= texturaJugador.getPosition();
       pos={(double)(88+60*10+10*0.2),20};
       texturaJugador.setPosition(pos);
       std::cout<<pos.getX()<<" "<<pos.getY()<<"\n";
    }
    else if(indexPosition>20){
       Vector2 pos= texturaJugador.getPosition();
       pos={(double)(88+60*(indexPosition%20)+(indexPosition%20)*0.2),20};
       texturaJugador.setPosition(pos);
       std::cout<<pos.getX()<<" "<<pos.getY()<<"\n";
    }
    else if(indexPosition==20){
       Vector2 pos= texturaJugador.getPosition();
       pos={50,(double)(495-45*10)};
       texturaJugador.setPosition(pos);
       std::cout<<pos.getX()<<" "<<pos.getY()<<"\n";
    }
    else if(indexPosition>10){
       Vector2 pos= texturaJugador.getPosition();
       pos={50,(double)(495-45*(indexPosition%10))};
       texturaJugador.setPosition(pos);
       std::cout<<pos.getX()<<" "<<pos.getY()<<"\n";
    }
    else if(indexPosition==10){
       if(!isInJail)texturaJugador.setPosition({25,500});
       else texturaJugador.setPosition({50,520});
    }
    else if(indexPosition==0){
        texturaJugador.setPosition({693.5,520});
    }
    else {
       Vector2 pos= texturaJugador.getPosition();
       pos={(double)(693.5-60*(indexPosition%40)-(indexPosition%40)*0.2),520};
       texturaJugador.setPosition(pos);
       std::cout<<pos.getX()<<" "<<pos.getY()<<"\n";
    }
}
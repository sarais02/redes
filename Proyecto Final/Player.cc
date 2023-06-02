#include "Player.h"
#include <cstdlib>
#include <ctime>
#include "Message.h"
#include <sstream>
#include <thread>
#include <random>
#include "Button.h"
void Player::login(){
    std::string msg;
    std::srand(std::time(0));
    IN_OUT em(nick, msg,true);   

    socket.send(em, socket);
    loadWindow();
    
    int max = nombresImagenes.size();
    randomss = std::vector<int>(max, -1);
    for (int i = 0; i < max; i++){
        // ASIGNACION TURNOS
        int rand = std::rand() % max;
        while (randomss[rand] != -1)
            rand = std::rand() % max;
        randomss[rand] = i;
    }
}
Player::~Player(){
   for (auto it=botones.begin();it!=botones.end();it++){
        delete it->second;
    }
}
void Player::logout(){
    // Completar
    std::string msg;

    IN_OUT em(nick, msg,false);
    if(onGame){
        for(auto it:playerProperties){
            HipotecaMsg hipoteca(it.first,1);
            socket.send(hipoteca,socket);
        }
        onGame=false;
    }
    socket.send(em, socket);
}

void Player::loadWindow(){
    SDLUtils::init("Monopoly Online", 800, 600);

    texturaMapa=SDLTexture(SDLUtils::instance()->renderer(), "Images/Tablero_1.jpg",{20,0,760,560});      
    texturaJugador=SDLTexture(SDLUtils::instance()->renderer(), "Images/Barco.png",{20,20,25,25});  
    texturaJugador.setPosition({693.5,520});

    miTurno=SDLTexture(SDLUtils::instance()->renderer(), "Images/miTurno.png",{350,100,100,100});
    miTurno.setVisibility(false);

    botones.insert({"comprarBoton",new Button(SDLUtils::instance()->renderer(), "Images/comprarBoton.png",{50,480,60,60}, this, &Player::comrarCalle)});
    botones["comprarBoton"]->setVisibility(false);

    botones.insert({"dados",new Button(SDLUtils::instance()->renderer(), "Images/dados.png",{620,420,60,60}, this, &Player::tirarDados)});
    botones["dados"]->setVisibility(false);

    mensajeCARCEL=SDLTexture(SDLUtils::instance()->renderer(), "Images/mensajeCarcel.png",{200,100,350,100});
    mensajeCARCEL.setVisibility(false);

    botones.insert({"CARCEL_NO",new Button(SDLUtils::instance()->renderer(), "Images/NO.png",{250,175,50,50},this,&Player::carcelNo)});
    botones["CARCEL_NO"]->setVisibility(false);

    botones.insert({"CARCEL_SI",new Button(SDLUtils::instance()->renderer(), "Images/SI.png",{450,175,50,50},this,&Player::carcelSi)});
    botones["CARCEL_SI"]->setVisibility(false);

}

void Player::input_thread(){
    std::srand(std::time(0));
    while (!exit){    
        while(onGame &&!exit){   
            // Leer stdin con std::getline
            // Enviar al servidor usando socket
            std::string msg;
            std::getline(std::cin,msg);        
            if(msg=="q"){
                logout();
                break;
            }
            if(isMyTurn){                              
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
                if(msg=="money"){
                    std::cout<<"Mi dinero: "<<money<<"\n";
                }                                          
            }
        }        
    }
    std::cout<<"HILO PRINCIPAL DEL PROGRAMA CERRADO\n";
}

void Player::net_thread(){
    while(!exit)
    {
        //Recibir Mensajes de red
        Message tmpMessage;
        socket.recv(tmpMessage);

        switch (tmpMessage.getType()){
            case MOVER:{
                PlayerSerializable playerMSG;
                playerMSG.from_bin(tmpMessage.data());
                if(playerMSG.indexPlayer==indexPlayer || indexPlayer==-1){
                    if(indexPlayer==-1)onGame=true;
                    indexPosition=playerMSG.indexPosition;
                    indexPlayer=playerMSG.indexPlayer;
                    money=playerMSG.dinero;
                    std::cout<<"JUGADO MOVIDO "<<indexPlayer<<"\n";
                    moverPlayer(indexPosition,&texturaJugador);                                      
                }
                else{
                    std::cout<<"RECIBO UN PLAYER"<<playerMSG.indexPlayer<<"\n";
                    players[playerMSG.indexPlayer]=playerMSG;
                    if(jugadores[playerMSG.indexPlayer].getSDLTex()==nullptr){
                        std::cout<<randomss.back()<<" "<<nombresImagenes[randomss.back()]<<"\n";
                        jugadores[playerMSG.indexPlayer]=SDLTexture(SDLUtils::instance()->renderer(), nombresImagenes[randomss.back()].data(),{20,20,20,20});
                        if(randomss.size()>1)randomss.pop_back();
                    }   
                    moverPlayer(playerMSG.indexPosition,&jugadores[playerMSG.indexPlayer]);                  
                }
                break; 
            }
            case INITURN:{
                std::cout<<"MI TURNO\n";
                if(isInJail){
                    cont++;
                    if(cont>=3) //Si ya lleva 3 turnos en la carcel sale
                    {
                        cont=0;
                        isMyTurn=true;
                        canFinishMyTurn=false;
                        isInJail=false; 
                        std::cout<<"SALGO DE LA CARCEL\n";
                        botones["dados"]->setVisibility(true);  
                        miTurno.setVisibility(true);                      
                    }
                    else //Si no acabo mi turno
                    {
                        isMyTurn=false;
                        canFinishMyTurn=false;
                        std::cout<<"Acabo Mi Turno\n";
                        Message initturno;
                        initturno.setType(ENDTURN);
                        socket.send(initturno, socket);
                        canBuySomething=false;                        
                    }
                }
                else{
                    botones["dados"]->setVisibility(true);  
                    miTurno.setVisibility(true);
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

                moverPlayer(compra.indexCasilla,botones["comprarBoton"],{60,20}); 
                botones["comprarBoton"]->setVisibility(true);      
                //GUARDARME LO K PUEDO COMPRAR
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
                //std::cout<<"Quieres pagar "<<moneyToPay<<
                //" para salir de la carcel? PRESS S TO ACCEPT OR N TO REJECT \n";
                mensajeCARCEL.setVisibility(true);
                botones["CARCEL_SI"]->setVisibility(true);
                botones["CARCEL_NO"]->setVisibility(true);
                break;
            }
            case CASA:{
                CasaMsg casa;
                casa.from_bin(tmpMessage.data());
                if(playerProperties.find(casa.indexPosition)!=playerProperties.end())
                     std::cout<<"SOLICITUD DE CASA: "<<casa.msgResponse;              
                //Poner casas en el mapa
                if(casa.quitarCasas==3){std::cout<<"quitar\n"; colocarCasas(casa.indexPosition,casa.numCasas,true);}
                else if(casa.quitarCasas==2){ std::cout<<"poner\n"; colocarCasas(casa.indexPosition,casa.numCasas,false);}
                break;
            }
            case HIPOTECA:{
                HipotecaMsg hipoteca;
                hipoteca.from_bin(tmpMessage.data());
                if(playerProperties.find(hipoteca.indexPosition)!=playerProperties.end())
                    std::cout<<"SOLICITUD DE HIPOTECAR: "<<hipoteca.msgResponse<<"\n";

                if(marcaHipoteca[hipoteca.indexPosition].getSDLTex()==nullptr){
                    marcaHipoteca[hipoteca.indexPosition]=SDLTexture(SDLUtils::instance()->renderer(), "Images/marcahipoteca.png",{20,20,30,30});
                    moverPlayer(hipoteca.indexPosition,&marcaHipoteca[hipoteca.indexPosition]);
                    marcaHipoteca[hipoteca.indexPosition].setVisibility(false);
                }

                if(hipoteca.hipoteca==2){
                    marcaHipoteca[hipoteca.indexPosition].setVisibility(true);    
                }
                else if(hipoteca.hipoteca==3){ 
                    marcaHipoteca[hipoteca.indexPosition].setVisibility(false); 
                }
                break;
            }
            case VICTORIA:{
                onGame=false;
                cout<<"HAS GANADO\n";
                win_or_lose= SDLTexture(SDLUtils::instance()->renderer(), "Images/HASGANADO.png",{350,100,100,100});
            }
        }        
    }
}

void Player::bucleVentana(){
    while(!exit){
        Uint32 startTime = sdlutils().currRealTime();
        gestionEventos();
        if(sdlutils().instance()->renderer()==nullptr)break;
        //RENDERIZAR IMAGENES       
        sdlutils().clearRenderer({0, 0, 0}); 

        texturaMapa.render();
        texturaJugador.render();
        miTurno.render();
        mensajeCARCEL.render();

        if(win_or_lose.getSDLTex()!=nullptr)win_or_lose.render();

        for(auto it=jugadores.begin();it!=jugadores.end();it++){
            it->second.render();
        }         
        for (auto it=botones.begin();it!=botones.end();it++){
           it->second->render();
        }
        for (auto it=marcaHipoteca.begin();it!=marcaHipoteca.end();it++){
           it->second.render();
        }
        for (auto it=casasCasilla.begin();it!=casasCasilla.end();it++){
            for(int i=0;i<it->second.size();i++){
                it->second[i].render();
            }             
        }      

        sdlutils().presentRenderer();//TE PRESENTA LOS CAMBIOS EN LA VENTANA
        Uint32 frameTime = sdlutils().currRealTime() - startTime;
		if (frameTime < 20)
			SDL_Delay(20 - frameTime);
    }
}

void Player::gestionEventos(){
    SDL_Event event;
    while (SDL_PollEvent(&event)){
        for (auto it=botones.begin();it!=botones.end();it++){
          it->second->handleEvent(event);
        }
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)){
            exit=true;
            sdlutils().closeWindow();
            logout();              
        }
        if(event.type == SDL_KEYDOWN&&onGame){
            switch (event.key.keysym.scancode){
                case SDL_SCANCODE_SPACE:{ //TIRAR LOS DADOS
                    if(isMyTurn&&!canFinishMyTurn){ //TIRAR DADOS
                        botones["dados"]->setVisibility(false);                                
                        int dado1 = std::rand() %6 + 1;
                        int dado2 = std::rand() %6 + 1;
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
                        moverPlayer(indexPosition,&texturaJugador);
                    }
                    break;
                }
                case SDL_SCANCODE_E:{
                    terminarTurno();
                    break;
                }
            default:
                break;
            }
        }
    }
}

void Player::moverPlayer(int indexPosition,SDLTexture* textura,Vector2 Offset){

    if(indexPosition>30){
        Vector2 pos= textura->getPosition();
        pos={(double)(720-Offset.getX()*1.75),(double)(45+45*(indexPosition%30)-Offset.getY())};
        textura->setPosition(pos);
    }
    else if(indexPosition==30){
    Vector2 pos= textura->getPosition();
       pos={(double)(88+60*10+10*0.2),20};
       textura->setPosition(pos);
    }
    else if(indexPosition>20){
       Vector2 pos= textura->getPosition();
       pos={(double)(88+60*(indexPosition%20)+(indexPosition%20)*0.2-Offset.getY()),(double)(20+Offset.getX()*1.25)};
       textura->setPosition(pos);
    }
    else if(indexPosition==20){
       Vector2 pos= textura->getPosition();
       pos={50,(double)(495-45*10)};
       textura->setPosition(pos);
    }
    else if(indexPosition>10){
       Vector2 pos= textura->getPosition();
       pos={(double)(50+Offset.getX()*1.5),(double)(495-45*(indexPosition%10)+Offset.getY()*0.25)};
       textura->setPosition(pos);
    }
    else if(indexPosition==10){
       if(!isInJail)textura->setPosition({25,500});
       else textura->setPosition({50,520});
    }
    else if(indexPosition==0){
        textura->setPosition({693.5,520});
    }
    else {
       Vector2 pos= textura->getPosition();
       pos={(double)(693.5-60*(indexPosition%40)-(indexPosition%40)*0.2)-Offset.getY(),(double)(520-Offset.getX()*1.25)};
       textura->setPosition(pos);
    }
}

void Player::comrarCalle(){
   if(isMyTurn && canBuySomething && compra.buyPrice<=money){ //SI PUEDO COMPRAR 
        botones["comprarBoton"]->setVisibility(false);
        canBuySomething=false;
        socket.send(compra,socket);                
        money-=compra.buyPrice;
        playerProperties.insert({compra.indexCasilla,compra.nombre});
        botones.insert({compra.nombre,new Button(SDLUtils::instance()->renderer(), "Images/hipotecar.png",{620,420,40,20}, this, nullptr,&Player::hipotecar)});
        botones[compra.nombre]->setIndex(compra.indexCasilla);
        moverPlayer(compra.indexCasilla,botones[compra.nombre],{50,15});
          
        std::cout<<"Calle Comprada\n";
                                    
    }
    else if(isMyTurn&& canBuySomething && compra.buyPrice>money){
        std::cout<<"No puedes comprar\n";
    }
}

void Player::tirarDados(){
    if(isMyTurn&&!canFinishMyTurn){ //TIRAR DADOS 
        botones["dados"]->setVisibility(false);                             
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
        moverPlayer(indexPosition,&texturaJugador);
    }
}

void Player::hipotecar(int index,bool wasclicked){
    if(isMyTurn){      
        int aux=!wasclicked?1:0;

        HipotecaMsg hipoteca(index,aux);
        socket.send(hipoteca,socket);
    }
}

void Player::colocarCasas(int index,int num,bool quitar){
    if(!quitar){
        for (int i = 0; i < num; i++){
           casasCasilla[index].push_back(SDLTexture(SDLUtils::instance()->renderer(), "Images/casa.png",{350,100,15,15}));
           moverPlayer( index,&casasCasilla[index].back(),{32.0,(double)(5-i*9)});
        }               
    }
    else{
        for (int i = 0; i < num; i++){
           casasCasilla[index].pop_back();
        } 
    }
}

void Player::carcelSi(){
    if(isMyTurn&&  isInJail && money>=moneyToPay){
        isInJail=false;
        money-=moneyToPay;
        PagarMsg pagar(moneyToPay);
        pagar.setType(CONFIRMCARCEL);
        socket.send(pagar, socket);
        std::cout<<"Fianza Pagada\n";
        mensajeCARCEL.setVisibility(false);
        botones["CARCEL_SI"]->setVisibility(false);
        botones["CARCEL_NO"]->setVisibility(false);
        miTurno.setVisibility(false);
        isMyTurn=false;
        canBuySomething=false;
    }
               
}

void Player::carcelNo(){
    if(isMyTurn && isInJail){
        std::cout<<"Te quedas en la carcel\n";
        std::cout<<"Se acaba tu turno\n";
        terminarTurno();
    }
}

void Player::terminarTurno(){
    if(isMyTurn&&canFinishMyTurn){ //SI PUEDO ACABAR TURNO
                       
        std::cout<<"Acabo Mi Turno\n";
        miTurno.setVisibility(false);
        botones["comprarBoton"]->setVisibility(false);
        mensajeCARCEL.setVisibility(false);
        botones["CARCEL_SI"]->setVisibility(false);
        botones["CARCEL_NO"]->setVisibility(false);
        isMyTurn=false;
        Message initturno;
        initturno.setType(ENDTURN);
        socket.send(initturno, socket);  
        canBuySomething=false;
        if(money<0){
            onGame=false;
            std::cout<<"BANCA ROTA\n";
            for(auto it:playerProperties){
                HipotecaMsg hipoteca(it.first,1);
                socket.send(hipoteca,socket);
            }
            win_or_lose= SDLTexture(SDLUtils::instance()->renderer(), "Images/BANCAROTA.png",{350,100,100,100});
        }
        std::cout<<"Money: "<<money<<"\n";
    }       
}
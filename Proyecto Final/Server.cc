#include "Server.h"
#include <random>
#include "Message.h"

void Server::do_messages(){
   while (true){
        Message tmpMessage;      
        //Recibir Mensajes en y en socketfunción del tipo de mensaje
        Socket* socket_cliente;
        socket.recv(tmpMessage, &socket_cliente);                      
          
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
            case MOVER:{
                PlayerSerializable player;
                player.from_bin(tmpMessage.data());
                players[player.indexPlayer]=player;
                std::cout<<"Player se ha movido: "<<player.indexPosition<<"\n";                

                movementConsequences(player.indexPlayer);
                break;
            }
            case COMPRADA:{ // actualizar el estado del tablero
                ComprarCalleMsg comprar;
                comprar.from_bin(tmpMessage.data());
                players[turnos[indexTurno]].dinero-=comprar.buyPrice;
                comprarCasilla(comprar);
                break;
            }
            case PAGADO:{ //cuando el jugador paga puede terminar su turno
                canFinishTurn=true;
                Message canFinish;
                canFinish.setType(CANENDTURN);
                socket.send(canFinish, *socketsPlayers[turnos[indexTurno]]);
                break;
            }
            case ENDTURN:{ 
                canFinishTurn=false;
                indexTurno =indexTurno+1 % turnos.size();               
                Message initturno;
                initturno.setType(INITURN);
                socket.send(initturno, *socketsPlayers[turnos[indexTurno]]);
                break;
            }
            
        }
        //}
        //else if(tmp.size()==PlayerSerializable::MESSAGE_SIZE){
        //    PlayerSerializable player;
        //    player.from_bin(tmp.data());
        //}
    }
};

void Server::input_thread(){
    while(true){
        std::string msg;
        std::getline(std::cin,msg);      
        if(msg=="start"&&!onGame){
            //if (players.size()<=1) std::cout<<"Se necesita mas de 1 jugador para empezar\n";
            //else{
                onGame=true;
                initTablero();
                initPlayers();
            //}
        }
    }
};
void Server::initTablero(){
    tablero = std::vector<Casilla*>(40);
    for(int i=0; i<40;i++){
        tablero[i] = new Casilla();
    }
    
    delete tablero[0]; 
    tablero[0]=new Salida("Salida", Type::SALIDA, 200);
    delete tablero[1];
    tablero[1]= new Calle("Ronda De Valencia",Type::CALLE,std::vector<int>({2,10,30,90,160,250}),60,30,50,std::vector<int>({3}));
    tablero[3]= new Calle("Plaza Lavapies",Type::CALLE,std::vector<int>({4,20,60,180,320,450}),60,30,50,std::vector<int>({1}));
    
    delete tablero[4];
    tablero[4]=new Impuesto("Impuesto sobre el capital", Type::IMPUESTO, 200);
    delete tablero[6]; delete tablero[8];delete tablero[9];
    tablero[6]= new Calle("Glorieta Cuatro Caminos",Type::CALLE,std::vector<int>({6,30,90,270,400,550}),100,50,50,std::vector<int>({8,9}));
    tablero[8]= new Calle("Avenida Reina Victoria",Type::CALLE,std::vector<int>({6,30,90,270,400,550}),100,50,50,std::vector<int>({6,9}));
    tablero[9]= new Calle("Calle Bravo Murillo",Type::CALLE,std::vector<int>({8,40,100,300,450,600}),120,60,50,std::vector<int>({6,8}));

    delete tablero[11]; delete tablero[13];delete tablero[14];
    tablero[11]= new Calle("Glorieta De Bilbao",Type::CALLE,std::vector<int>({10,50,150,450,625,750}),140,70,100,std::vector<int>({13,14}));
    tablero[13]= new Calle("Calle Alberto Aguilera",Type::CALLE,std::vector<int>({10,50,150,450,625,750}),140,70,100,std::vector<int>({11,14}));
    tablero[14]= new Calle("Calle Fuencarral",Type::CALLE,std::vector<int>({12,60,180,500,700,900}),160,80,100,std::vector<int>({13,11}));

    delete tablero[16]; delete tablero[18];delete tablero[19];
    tablero[16]= new Calle("Avenida Felipe II",Type::CALLE,std::vector<int>({14,70,200,550,750,950}),180,90,100,std::vector<int>({18,19}));
    tablero[18]= new Calle("Calle Velazquez",Type::CALLE,std::vector<int>({14,70,200,550,750,950}),180,90,100,std::vector<int>({16,19}));
    tablero[19]= new Calle("Calle Serrano",Type::CALLE,std::vector<int>({16,80,220,600,800,1000}),200,100,100,std::vector<int>({16,18}));

    delete tablero[21]; delete tablero[23];delete tablero[24];
    tablero[21]= new Calle("Avenida De America",Type::CALLE,std::vector<int>({18,90,250,700,875,1050}),220,110,150,std::vector<int>({23,24}));
    tablero[23]= new Calle("Calle Maria De Molina",Type::CALLE,std::vector<int>({18,90,250,700,875,1050}),220,110,150,std::vector<int>({21,24}));
    tablero[24]= new Calle("Calle Cea Bermudez",Type::CALLE,std::vector<int>({20,100,300,750,925,1100}),240,120,150,std::vector<int>({21,23}));

    delete tablero[26]; delete tablero[28];delete tablero[29];
    tablero[26]= new Calle("Avenida De Los Reyes Catolicos",Type::CALLE,std::vector<int>({22,110,330,800,975,1150}),260,130,150,std::vector<int>({28,29}));
    tablero[28]= new Calle("Calle Bailen",Type::CALLE,std::vector<int>({22,110,330,800,975,1150}),260,130,150,std::vector<int>({26,29}));
    tablero[29]= new Calle("Plaza De España",Type::CALLE,std::vector<int>({24,120,360,850,1025,1200}),280,140,150,std::vector<int>({26,28}));

    delete tablero[31]; delete tablero[32];delete tablero[34];
    tablero[26]= new Calle("Puerta Del Sol",Type::CALLE,std::vector<int>({26,130,390,900,1100,1275}),300,150,200,std::vector<int>({28,29}));
    tablero[28]= new Calle("Calle Alcala",Type::CALLE,std::vector<int>({26,130,390,900,1100,1275}),300,150,200,std::vector<int>({26,29}));
    tablero[29]= new Calle("Gran Via",Type::CALLE,std::vector<int>({28,150,450,1000,1200,1400}),320,160,200,std::vector<int>({26,28}));

    delete tablero[37]; delete tablero[38]; delete tablero[39];
    tablero[37]= new Calle("Paseo De La Castellana",Type::CALLE,std::vector<int>({35,175,500,1100,1300,1500}),350,175,200,std::vector<int>({39}));
    tablero[38] = new Impuesto("Impuesto de lujo", Type::IMPUESTO, 100);
    tablero[39]= new Calle("Paseo Del Prado",Type::CALLE,std::vector<int>({50,200,600,1400,1700,2000}),400,200,200,std::vector<int>({37}));

};

void Server::initPlayers(){
    
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
};

void Server::movementConsequences(int indexPlayer){
    int posPlayer= players[indexPlayer].indexPosition;

    switch (tablero[posPlayer]->getType()){
        case CALLE:{
            Calle* calle = dynamic_cast<Calle*>(tablero[posPlayer]);
            
            if(calle->getProperty()==-1){ //Se puede Comprar

                canFinishTurn=true;
                Message canFinish;               
                canFinish.setType(CANENDTURN);
                socket.send(canFinish, *socketsPlayers[turnos[indexTurno]]);

                ComprarCalleMsg comprarMsg(calle->getName(),indexPlayer,calle->getPrice());
                comprarMsg.setType(COMPRAR);
                socket.send(comprarMsg, *socketsPlayers[turnos[indexTurno]]);
                std::cout<<"SOLICITUD DE COMPRA ENVIADA A PLAYER: "<<players[indexPlayer].nick<<"\n";
            }
            else if(calle->getProperty()!=indexPlayer){ //La tiene alguien

            }
            else std::cout<<"LA CALLE YA ES PROPIEDAD DEL PLAYER "<<players[indexPlayer].nick<<"\n";
            
            break;
        }
        case IMPUESTO:{
            Impuesto* impuesto=dynamic_cast<Impuesto*>(tablero[posPlayer]);
            PagarMsg p(impuesto->getPrice());
            p.setType(PAGAR);
            socket.send(p, *socketsPlayers[turnos[indexTurno]]);
            std::cout<<"EL JUGADOR: "<<players[indexPlayer].nick<<" DEBE PAGAR: " << impuesto->getName() << "\n";

            canFinishTurn=true;
            Message canFinish;               
            canFinish.setType(CANENDTURN);
            socket.send(canFinish, *socketsPlayers[turnos[indexTurno]]);
            break;
        }
        case SALIDA:{
            Salida* salida=dynamic_cast<Salida*>(tablero[posPlayer]);
            PagarMsg p(salida->getReward());
            p.setType(COBRAR);
            socket.send(p, *socketsPlayers[turnos[indexTurno]]);
            std::cout<<"EL JUGADOR: "<<players[indexPlayer].nick<<" COBRA: " << salida->getReward() << "\n";
            
            canFinishTurn=true;
            Message canFinish;               
            canFinish.setType(CANENDTURN);
            socket.send(canFinish, *socketsPlayers[turnos[indexTurno]]);
            break;
        }
        default: {//TEMPORAL HAY QUITARLO ESTO ES SOLO PARA Q LAS CASILLAS EN LAS K ACTUIALMENTE NO HAY NADA SE PUEDA ACABAR TURNO
            canFinishTurn=true;
            Message canFinish;               
            canFinish.setType(CANENDTURN);
            socket.send(canFinish, *socketsPlayers[turnos[indexTurno]]);
            break;
        }
       
    }

};

void Server::comprarCasilla(ComprarCalleMsg comprar){
    int indexplayer= turnos[indexTurno];
    switch (tablero[comprar.indexCasilla]->getType()){
        case CALLE:{
            Calle* calle = dynamic_cast<Calle*>(tablero[comprar.indexCasilla]);
            calle->setProperty(indexplayer);
            std::cout<<"CASILLA COMPRADA POR "<<players[indexplayer].nick<<"\n";
            break;           
        }
    }
}
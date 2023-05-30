#include "Server.h"
#include <random>
#include "Message.h"

void Server::do_messages()
{
    while (true)
    {
        Message tmpMessage;
        // Recibir Mensajes en y en socketfunción del tipo de mensaje
        Socket *socket_cliente;
        int x = socket.recv(tmpMessage, &socket_cliente);

        switch (tmpMessage.getType())
        {
        case LOGIN:
        {
            IN_OUT in;
            in.from_bin(tmpMessage.data());
            std::unique_ptr<Socket> socket1_(socket_cliente);
            socketsPlayers.push_back(std::move(socket1_));
            std::cout << "LOGIN DE: " << in.nick << "\n";

            PlayerSerializable tmpPLayer(in.nick, 0, 1500, (int)(socketsPlayers.size() - 1));
            players.push_back(tmpPLayer);

            break;
        }
        case LOGOUT:
        {
            IN_OUT out;
            out.from_bin(tmpMessage.data());
            std::unique_ptr<Socket> socket_(socket_cliente);
            for (auto it = socketsPlayers.begin(); it != socketsPlayers.end();)
            {
                if (*(*it) == *socket_cliente)
                {
                    socketsPlayers.erase(it);
                }
                else
                    ++it;
            }
            std::cout << "LOGOUT DE: " << out.nick << "\n";
            break;
        }
        case MOVER:
        {
            PlayerSerializable player;
            player.from_bin(tmpMessage.data());
            players[player.indexPlayer] = player;
            std::cout << "Player se ha movido: " << player.indexPosition << "\n";

            movementConsequences(player.indexPlayer);
            break;
        }
        case COMPRAR:
        { // actualizar el estado del tablero
            ComprarCalleMsg comprar;
            comprar.from_bin(tmpMessage.data());
            players[turnos[indexTurno]].dinero -= comprar.buyPrice;
            comprarCasilla();
            break;
        }
        case PAGADO:
        { // cuando el jugador paga puede terminar su turno
            std::cout << "PAGADO\n";
            canFinishTurn = true;
            Message canFinish;
            canFinish.setType(CANENDTURN);
            socket.send(canFinish, *socketsPlayers[turnos[indexTurno]]);
            break;
        }
        case ENDTURN:
        {
            std::cout << "ENDTURN\n";
            canFinishTurn = false;
            indexTurno = indexTurno + 1 % turnos.size();
            Message initturno;
            initturno.setType(INITURN);
            socket.send(initturno, *socketsPlayers[turnos[indexTurno]]);
            break;
        }
        case CASA:{
            CasaMsg casa;
            casa.from_bin(tmpMessage.data());
            if (tengoColor(casa.indexPosition)){
                if(tryPonerCasas(casa.indexPosition,casa.numCasas))
                    std::cout << "Se han puesto " << casa.numCasas << " en " << casa.indexPosition << "\n";
                else  ;             
            }
            else std::cout<<"NO SE HAN PODIDO PONER CASAS NO TIENE TODAS DEL MISMO COLOR\n";
            break;
        }
        }
    }
};

void Server::input_thread()
{
    while (true)
    {
        std::string msg;
        std::getline(std::cin, msg);
        if (msg == "start" && !onGame)
        {
            // if (players.size()<=1) std::cout<<"Se necesita mas de 1 jugador para empezar\n";
            // else{
            onGame = true;
            initTablero();
            initPlayers();
            //}
        }
    }
};
void Server::initTablero()
{
    tablero = std::vector<Casilla *>(40);
    for (int i = 0; i < 40; i++)
    {
        tablero[i] = new Casilla();
    }

    delete tablero[0];
    tablero[0] = new Salida("Salida", Type::SALIDA, 200);

    delete tablero[1];
    delete tablero[3];
    tablero[1] = new Calle("Ronda De Valencia", Type::CALLE, std::vector<int>({2, 10, 30, 90, 160, 250}), 60, 30, 50, std::vector<int>({3}));
    tablero[3] = new Calle("Plaza Lavapies", Type::CALLE, std::vector<int>({4, 20, 60, 180, 320, 450}), 60, 30, 50, std::vector<int>({1}));

    delete tablero[4];
    tablero[4] = new Impuesto("Impuesto sobre el capital", Type::IMPUESTO, 200);
    delete tablero[5];
    tablero[5] = new Calle("Estacion De Goya", Type::ESTACION, std::vector<int>({25, 50, 100, 200}), 200, 100, 0, std::vector<int>({15, 25, 35}));

    delete tablero[6];
    delete tablero[8];
    delete tablero[9];
    tablero[6] = new Calle("Glorieta Cuatro Caminos", Type::CALLE, std::vector<int>({6, 30, 90, 270, 400, 550}), 100, 50, 50, std::vector<int>({8, 9}));
    tablero[8] = new Calle("Avenida Reina Victoria", Type::CALLE, std::vector<int>({6, 30, 90, 270, 400, 550}), 100, 50, 50, std::vector<int>({6, 9}));
    tablero[9] = new Calle("Calle Bravo Murillo", Type::CALLE, std::vector<int>({8, 40, 100, 300, 450, 600}), 120, 60, 50, std::vector<int>({6, 8}));
    //La casilla 10 es la carcel, asi que sera una casilla de ningun tipo
    delete tablero[11];
    delete tablero[13];
    delete tablero[14];
    tablero[11] = new Calle("Glorieta De Bilbao", Type::CALLE, std::vector<int>({10, 50, 150, 450, 625, 750}), 140, 70, 100, std::vector<int>({13, 14}));
    tablero[13] = new Calle("Calle Alberto Aguilera", Type::CALLE, std::vector<int>({10, 50, 150, 450, 625, 750}), 140, 70, 100, std::vector<int>({11, 14}));
    tablero[14] = new Calle("Calle Fuencarral", Type::CALLE, std::vector<int>({12, 60, 180, 500, 700, 900}), 160, 80, 100, std::vector<int>({13, 11}));

    delete tablero[15];
    tablero[15] = new Calle("Estacion De Las Delicias", Type::ESTACION, std::vector<int>({25, 50, 100, 200}), 200, 100, 0, std::vector<int>({5, 25, 35}));

    delete tablero[16];
    delete tablero[18];
    delete tablero[19];
    tablero[16] = new Calle("Avenida Felipe II", Type::CALLE, std::vector<int>({14, 70, 200, 550, 750, 950}), 180, 90, 100, std::vector<int>({18, 19}));
    tablero[18] = new Calle("Calle Velazquez", Type::CALLE, std::vector<int>({14, 70, 200, 550, 750, 950}), 180, 90, 100, std::vector<int>({16, 19}));
    tablero[19] = new Calle("Calle Serrano", Type::CALLE, std::vector<int>({16, 80, 220, 600, 800, 1000}), 200, 100, 100, std::vector<int>({16, 18}));

    delete tablero[21];
    delete tablero[23];
    delete tablero[24];
    tablero[21] = new Calle("Avenida De America", Type::CALLE, std::vector<int>({18, 90, 250, 700, 875, 1050}), 220, 110, 150, std::vector<int>({23, 24}));
    tablero[23] = new Calle("Calle Maria De Molina", Type::CALLE, std::vector<int>({18, 90, 250, 700, 875, 1050}), 220, 110, 150, std::vector<int>({21, 24}));
    tablero[24] = new Calle("Calle Cea Bermudez", Type::CALLE, std::vector<int>({20, 100, 300, 750, 925, 1100}), 240, 120, 150, std::vector<int>({21, 23}));

    delete tablero[25];
    tablero[25] = new Calle("Estacion Del Mediodia", Type::ESTACION, std::vector<int>({25, 50, 100, 200}), 200, 100, 0, std::vector<int>({5, 15, 35}));

    delete tablero[26];
    delete tablero[28];
    delete tablero[29];
    tablero[26] = new Calle("Avenida De Los Reyes Catolicos", Type::CALLE, std::vector<int>({22, 110, 330, 800, 975, 1150}), 260, 130, 150, std::vector<int>({28, 29}));
    tablero[28] = new Calle("Calle Bailen", Type::CALLE, std::vector<int>({22, 110, 330, 800, 975, 1150}), 260, 130, 150, std::vector<int>({26, 29}));
    tablero[29] = new Calle("Plaza De España", Type::CALLE, std::vector<int>({24, 120, 360, 850, 1025, 1200}), 280, 140, 150, std::vector<int>({26, 28}));
    delete tablero[30];
    tablero[30]=new Carcel("Ve a la carcel", Type::CARCEL, 50);
    delete tablero[31];
    delete tablero[32];
    delete tablero[34];
    tablero[31] = new Calle("Puerta Del Sol", Type::CALLE, std::vector<int>({26, 130, 390, 900, 1100, 1275}), 300, 150, 200, std::vector<int>({32, 34}));
    tablero[32] = new Calle("Calle Alcala", Type::CALLE, std::vector<int>({26, 130, 390, 900, 1100, 1275}), 300, 150, 200, std::vector<int>({31, 34}));
    tablero[34] = new Calle("Gran Via", Type::CALLE, std::vector<int>({28, 150, 450, 1000, 1200, 1400}), 320, 160, 200, std::vector<int>({31, 32}));

    delete tablero[35];
    tablero[35] = new Calle("Estacion Del Norte", Type::ESTACION, std::vector<int>({25, 50, 100, 200}), 200, 100, 0, std::vector<int>({5, 15, 25}));

    delete tablero[37];
    delete tablero[38];
    delete tablero[39];
    tablero[37] = new Calle("Paseo De La Castellana", Type::CALLE, std::vector<int>({35, 175, 500, 1100, 1300, 1500}), 350, 175, 200, std::vector<int>({39}));
    tablero[38] = new Impuesto("Impuesto de lujo", Type::IMPUESTO, 100);
    tablero[39] = new Calle("Paseo Del Prado", Type::CALLE, std::vector<int>({50, 200, 600, 1400, 1700, 2000}), 400, 200, 200, std::vector<int>({37}));
};

void Server::initPlayers()
{

    int max = socketsPlayers.size();
    turnos = std::vector<int>(max, -1);
    for (int i = 0; i < max; i++)
    {
        // ASIGNACION TURNOS
        int rand = std::rand() % max;
        while (turnos[rand] != -1)
            rand = std::rand() % max;
        turnos[rand] = i;
    }
    int i = 0;
    for (auto it = socketsPlayers.begin(); it != socketsPlayers.end(); ++it)
    {
        if ((*it) == nullptr)
            std::cout << "NULL\n";
        socket.send(players[i], *(*it));
        std::cout << "ENVIADO " << players[i].indexPlayer << "\n";
        i++;
    }
    Message initturno;
    initturno.setType(INITURN);
    socket.send(initturno, *socketsPlayers[turnos[indexTurno]]);
    std::cout << "ENVIADO TURNO A: " << turnos[indexTurno] << "\n";
};

void Server::movementConsequences(int indexPlayer)
{
    int posPlayer = players[indexPlayer].indexPosition;

    switch (tablero[posPlayer]->getType())
    {
    case ESTACION:
    case CALLE:
    {
        Calle *calle = dynamic_cast<Calle *>(tablero[posPlayer]);

        if (calle->getProperty() == -1) // Se puede Comprar
        { 

            ComprarCalleMsg comprarMsg(calle->getName(), posPlayer, calle->getPrice());
            comprarMsg.setType(COMPRAR);
            socket.send(comprarMsg, *socketsPlayers[turnos[indexTurno]]);
            std::cout << "SOLICITUD DE COMPRA ENVIADA A PLAYER: " << players[indexPlayer].nick << "\n";

            canFinishTurn = true;
            Message canFinish;
            canFinish.setType(CANENDTURN);
            socket.send(canFinish, *socketsPlayers[turnos[indexTurno]]);
        }
        else if (calle->getProperty() != indexPlayer)// La tiene alguien
        { 
            int dinero=calle->rentCost(); //Dinero que se cobra
            //Mensaje para cobrar al que ha caido
            PagarMsg pagar(dinero); 
            pagar.setType(PAGAR);
            socket.send(pagar, *socketsPlayers[turnos[indexTurno]]);
            std::cout << "EL JUGADOR: " << players[indexPlayer].nick << " DEBE PAGAR: " << dinero << " DE ALQUILER\n";
            //Mensaje para que el propietario cobre
            PagarMsg cobrar(dinero);
            cobrar.setType(COBRAR);
            socket.send(cobrar, *socketsPlayers[calle->getProperty()]);
            std::cout << "EL JUGADOR: " << players[indexPlayer].nick << " DEBE PAGAR: " << dinero << " DE ALQUILER A: "<< players[calle->getProperty()].nick<<"\n";

            canFinishTurn = true;
            Message canFinish;
            canFinish.setType(CANENDTURN);
            socket.send(canFinish, *socketsPlayers[turnos[indexTurno]]);
        }
        else
            std::cout << "ESTA PROPIEDAD YA ES PROPIEDAD DEL PLAYER " << players[indexPlayer].nick << "\n";

        break;
    }
    case IMPUESTO:
    {
        Impuesto *impuesto = dynamic_cast<Impuesto *>(tablero[posPlayer]);
        PagarMsg p(impuesto->getPrice());
        p.setType(PAGAR);
        socket.send(p, *socketsPlayers[turnos[indexTurno]]);
        std::cout << "EL JUGADOR: " << players[indexPlayer].nick << " DEBE PAGAR: " << impuesto->getName() << "\n";

        ///// REVISAR, EL JUGADOR PUEDE ACABAR SU TURNO SIN HABER PAGADO
        canFinishTurn = true;
        Message canFinish;
        canFinish.setType(CANENDTURN);
        socket.send(canFinish, *socketsPlayers[turnos[indexTurno]]);
        break;
    }
    case SALIDA:
    {
        Salida *salida = dynamic_cast<Salida *>(tablero[posPlayer]);
        PagarMsg p(salida->getReward());
        p.setType(COBRAR);
        socket.send(p, *socketsPlayers[turnos[indexTurno]]);
        std::cout << "EL JUGADOR: " << players[indexPlayer].nick << " COBRA: " << salida->getReward() << "\n";

        canFinishTurn = true;
        Message canFinish;
        canFinish.setType(CANENDTURN);
        socket.send(canFinish, *socketsPlayers[turnos[indexTurno]]);
        break;
    }
    case CARCEL:
    {
        Carcel *carcel=dynamic_cast<Carcel*>(tablero[posPlayer]);
        //Muevo al jugador a la carcel
        PlayerSerializable player(players[indexPlayer].nick,10,players[indexPlayer].dinero,indexPlayer);
        socket.send(player,*socketsPlayers[turnos[indexTurno]]);
        //Mensaje de carcel
        CarcelMsg m(carcel->getPrice()); //Dinero que cuesta salir de la carcel
        m.setType(IRACARCEL);
        socket.send(m,*socketsPlayers[turnos[indexTurno]]);

        canFinishTurn = true;
        Message canFinish;
        canFinish.setType(CANENDTURN);
        socket.send(canFinish, *socketsPlayers[turnos[indexTurno]]);
        break;
    }
    default:
    { // QUITAR ESTO ES SOLO PARA LAS CASILLAS QUE NO TIENEN NADA
        canFinishTurn = true;
        Message canFinish;
        canFinish.setType(CANENDTURN);
        socket.send(canFinish, *socketsPlayers[turnos[indexTurno]]);
    }
    }
};

void Server::comprarCasilla()
{
    int indexplayer = turnos[indexTurno];
    switch (tablero[players[indexplayer].indexPosition]->getType())
    {
    case SERVICIOS:
    case CALLE:
    {
        Calle *calle = dynamic_cast<Calle *>(tablero[players[indexplayer].indexPosition]);
        calle->setProperty(indexplayer);
        std::cout << "Casilla " << calle->getName() << " comprada por " << players[indexplayer].nick << "\n";
        break;
    }
    case ESTACION:
    {
        Calle *calle = dynamic_cast<Calle *>(tablero[players[indexplayer].indexPosition]);
        calle->setProperty(indexplayer);
        std::cout << calle->getName() << " comprada por " << players[indexplayer].nick << "\n";
        int numEstaciones = 0;
        std::vector<Calle *> propsPlayer;
        for (int i = 5; i < 40; i += 10)
        {
            Calle *calle = dynamic_cast<Calle *>(tablero[i]);
            if (calle->getProperty() == indexplayer)
            {
                propsPlayer.push_back(calle);
                numEstaciones++;
            }
        }
        numEstaciones--; // pq el vector de rentas empieza en 0
        for (int i = 0; i < propsPlayer.size(); i++)
        {
            propsPlayer[i]->setRentIndex(numEstaciones);
        }
        break;
    }
    }
}

bool Server::tryPonerCasas(int indexPosition, int numCasas){
    // todas las casillas las tiene el jugador
    Calle *calle = dynamic_cast<Calle *>(tablero[indexPosition]); 
    if(calle->getHousePrice()*numCasas<=players[turnos[indexTurno]].dinero){ 
        players[turnos[indexTurno]].dinero-=calle->getHousePrice()*numCasas;
        calle->setRentIndex(numCasas);
        return true; 
    }
    return false;
};
bool Server::tengoColor(int indexPosition){
    Casilla *tmpCasilla = tablero[indexPosition];
    if (tmpCasilla->getType() == CALLE)
    {
        Calle *calle = dynamic_cast<Calle *>(tablero[indexPosition]);
        for (auto it : calle->getFamily())
        {
            if (dynamic_cast<Calle *>(tablero[indexPosition])->getProperty() != turnos[indexTurno])
            {
                return false;
            }
        }
        return true;
    }
    return false;
}
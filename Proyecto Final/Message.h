#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "Serializable.h"
#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <memory>


enum TypesMessages : u_int8_t {
    EMPTY=0,
    LOGIN=1,
    LOGOUT=2,
    MOVER=3,
    INITURN=4,
    ENDTURN=5,
    COMPRAR=6,
    PAGAR=8,
    PAGADO=9,
    CANENDTURN=10,
    COBRAR=11,
    CASA=12
};
class Message:public Serializable{
public:
    static const size_t MESSAGE_SIZE = 1024;
    Message();
    virtual void to_bin() override;
    virtual int from_bin(char * bobj) override;
    u_int8_t getType();
    void setType(u_int8_t type);
protected:
    u_int8_t type;
};
/**
 *  Mensaje del protocolo de la aplicación de Chat
 *
 *  +-------------------+
 *  | Tipo: uint8_t     | 0 (login), 1 (mensaje), 2 (logout)
 *  +-------------------+
 *  | Nick: char[8]     | Nick incluido el char terminación de cadena '\0'
 *  +-------------------+
 *  |                   |
 *  | Mensaje: char[80] | Mensaje incluido el char terminación de cadena '\0'
 *  |                   |
 *  +-------------------+
 *
 */
class IN_OUT: public Message{
public:
    static const size_t MESSAGE_SIZE = sizeof(char) * 88 + sizeof(uint8_t);

    IN_OUT():Message(){};

    IN_OUT(const std::string& n, const std::string& m,bool in_out);

    void to_bin() override;

    int from_bin(char * bobj) override;

    std::string nick;
    std::string message;
};

/**
 *  Mensaje del protocolo de la aplicación 
 *  EL TIPO DE MENSAJE
 *  +-------------------+
 *  | Nick: char[8]     | Nick incluido el char terminación de cadena '\0'
 *  +-------------------+
 *  |                   |
 *  | int indexPosition | Posicion del player en el array de casillas
 *  |                   |
 *  +-------------------+
 *  |                   |
 *  | int dinero        | 
 *  |                   |
 *  +-------------------+
 *  | int indexPlayer   | 
 *  |                   |
 *  +-------------------+
 */
class PlayerSerializable: public Message{
public:
    static const size_t MESSAGE_SIZE = sizeof(uint8_t) + sizeof(char) * 8 + sizeof(int16_t) * 3;
    //PLAYER SERIALIZABLE POR DEFECTO TIENE EL TIPO DE MENSAJE MOVER
    PlayerSerializable():Message(){type=MOVER;};
    PlayerSerializable(std::string Nick, int16_t IndexPosition, int16_t Dinero, int16_t IndexPlayer);

    void to_bin() override;
    int from_bin(char * bobj) override;

    std::string nick;
    int16_t indexPosition;
    int16_t indexPlayer;
    int16_t dinero;
};

//OBJECTIVO DE ESTE TIPO DE MENSAJES ES EL INTERCAMBIO DE INFORMACION EN EL TRANSCURSO DE COMPRAR UNA PROPIEDAD
class ComprarCalleMsg: public Message{
public:
    static const size_t MESSAGE_SIZE = sizeof(uint8_t) + sizeof(char) * 24 + sizeof(int16_t)*2;
    //PLAYER SERIALIZABLE POR DEFECTO TIENE EL TIPO DE MENSAJE MOVER
    ComprarCalleMsg():Message(){type=COMPRAR;};
    ComprarCalleMsg(std::string Nombre, int16_t IndexCasilla, int16_t BuyPrice);

    void to_bin() override;
    int from_bin(char * bobj) override;

    std::string nombre;//MAXIMO APROXIMADO 24 CARACTERES POR CALLE ("casi todas menos pero por si acaso")
    int16_t buyPrice;
    int16_t indexCasilla;

};

class PagarMsg: public Message{
public:
    static const size_t MESSAGE_SIZE = sizeof(uint8_t)+ sizeof(int16_t);
    //PLAYER SERIALIZABLE POR DEFECTO TIENE EL TIPO DE MENSAJE MOVER
    PagarMsg():Message(){type=PAGAR;};
    PagarMsg(int16_t BuyPrice);

    void to_bin() override;
    int from_bin(char * bobj) override;
   
    int16_t buyPrice;
};

class CasaMsg: public Message{
public:
    static const size_t MESSAGE_SIZE = sizeof(uint8_t)+ sizeof(int16_t)*2;
    //PLAYER SERIALIZABLE POR DEFECTO TIENE EL TIPO DE MENSAJE MOVER
    CasaMsg():Message(){type=CASA;};
    CasaMsg(int16_t IndexPosition,int16_t NumCasas);

    void to_bin() override;
    int from_bin(char * bobj) override;
   
    int16_t indexPosition;
    int16_t numCasas;
    std::string msgResponse;
};
#endif
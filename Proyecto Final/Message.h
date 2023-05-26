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
    PLAYERSERIALIZABLE=3,
    INITURN=4,
    ENDTURN=5
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
 *
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

    PlayerSerializable():Message(){type=PLAYERSERIALIZABLE;};
    PlayerSerializable(std::string Nick, int16_t IndexPosition, int16_t Dinero, int16_t IndexPlayer);

    void to_bin() override;
    int from_bin(char * bobj) override;

    std::string nick;
    int16_t indexPosition;
    int16_t indexPlayer;
    int16_t dinero;
};
#endif
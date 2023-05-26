#ifndef CHATMESSAGE_H_
#define CHATMESSAGE_H_

#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <memory>

#include "Serializable.h"
#include "Socket.h"
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
class ChatMessage: public Serializable
{
public:
    static const size_t MESSAGE_SIZE = sizeof(char) * 88 + sizeof(uint8_t);

    enum MessageType
    {
        LOGIN   = 0,
        MESSAGE = 1,
        LOGOUT  = 2,
        THROW
    };

    ChatMessage(){};

    ChatMessage(const std::string& n, const std::string& m):nick(n),message(m){};

    void to_bin() override;

    int from_bin(char * bobj) override;

    uint8_t type;

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
class PlayerSerializable:public Serializable{
public:
    static const size_t MESSAGE_SIZE = sizeof(char) * 10 + sizeof(int16_t) * 3;

    PlayerSerializable(){};
    PlayerSerializable(std::string Nick, int16_t IndexPosition, int16_t Dinero, int16_t IndexPlayer):nick(Nick),indexPosition(IndexPosition),indexPlayer(IndexPlayer),dinero(Dinero){};

    void to_bin() override;
    int from_bin(char * bobj) override;

    std::string nick;
    int16_t indexPosition;
    int16_t indexPlayer;
    int16_t dinero;
};
#endif
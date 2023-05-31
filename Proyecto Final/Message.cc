#include "Message.h"
#include <iostream>
Message::Message():Serializable(),type(EMPTY){
};

void Message::to_bin(){
    alloc_data(sizeof(uint8_t));
    memset(_data, 0, sizeof(uint8_t));
    //Serializar los campos type, nick y message en el buffer _data
    char* buffer = _data;
    memcpy(buffer, &type, sizeof(uint8_t));
};

int Message::from_bin(char * bobj){
    alloc_data(MESSAGE_SIZE);
    memcpy(static_cast<void*>(_data), bobj, MESSAGE_SIZE);
    memcpy(&type,bobj, sizeof(uint8_t));
    //std::cout<<"Recibido mensaje de tipo: "<<type<<"\n";
    return 0;
};

u_int8_t Message::getType(){
    return type;
};

void Message::setType(u_int8_t type){
    this->type=type;
}

IN_OUT::IN_OUT(const std::string& n, const std::string& m,bool in_out):Message(),nick(n),message(m){
    type = in_out ? LOGIN: LOGOUT;
};

void IN_OUT::to_bin(){
    alloc_data(MESSAGE_SIZE);

    memset(_data, 0, MESSAGE_SIZE);
    //Serializar los campos type, nick y message en el buffer _data
    char* buffer = _data;
    memcpy(buffer, &type, sizeof(uint8_t));
    buffer += sizeof(uint8_t);
    memcpy(buffer, nick.c_str(), sizeof(char)* 8);
    buffer += sizeof(char) * 8;
    memcpy(buffer, message.c_str(), sizeof(char) * 80);
}

int IN_OUT::from_bin(char * bobj){
    alloc_data(MESSAGE_SIZE); 
    memcpy(static_cast<void*>(_data), bobj, MESSAGE_SIZE); 
    //Reconstruir la clase usando el buffer _data
    char* buffer = _data; 
    memcpy(&type, buffer, sizeof(uint8_t)); 
    buffer += sizeof(uint8_t); 
    char nick_buffer[8] = {0}; 
    memcpy(nick_buffer, buffer, 8* sizeof(char)); 
    nick = std::string(nick_buffer); 
    buffer += 8* sizeof(char); 
    char message_buffer[81] = {0};
    memcpy(message_buffer, buffer, 80 * sizeof(char)); 
    message = std::string(message_buffer); 
    return 0;
}

PlayerSerializable::PlayerSerializable(std::string Nick, int16_t IndexPosition, int16_t Dinero, int16_t IndexPlayer):
    nick(Nick),indexPosition(IndexPosition),indexPlayer(IndexPlayer),dinero(Dinero){
        type=MOVER;
    };

void PlayerSerializable::to_bin(){
    alloc_data(MESSAGE_SIZE);

    memset(_data, 0, MESSAGE_SIZE);
    //Serializar los campos type, nick y message en el buffer _data
    char* tmp = _data;
    memcpy(tmp, &type, sizeof(uint8_t));
    tmp += sizeof(uint8_t);
    memcpy(tmp, nick.c_str(), sizeof(char)* 8);   
    tmp += sizeof(char) * 8;
    //std::cout<<"NICK\n";
    memcpy(tmp, &indexPosition, sizeof(int16_t));
    tmp += sizeof(int16_t);
    //std::cout<<"POSITION\n";
    memcpy(tmp, &dinero, sizeof(int16_t));
    tmp += sizeof(int16_t);
    //std::cout<<"DINERO\n";
    memcpy(tmp, &indexPlayer, sizeof(int16_t));
    //std::cout<<"PLAYER\n";
}

int PlayerSerializable::from_bin(char * bobj){
    alloc_data(MESSAGE_SIZE);
    memcpy(static_cast<void*>(_data), bobj, MESSAGE_SIZE); 
    //Reconstruir la clase usando el buffer _data
    char* buffer = _data;
    memcpy(&type, buffer, sizeof(uint8_t)); 
    buffer += sizeof(uint8_t); 
    char nick_buffer[8]; 
    memcpy(&nick_buffer, buffer, 8 * sizeof(char)); 
    nick = std::string(nick_buffer); 
    buffer += 8* sizeof(char);
    memcpy(&indexPosition, buffer, sizeof(int16_t));
    buffer+=sizeof(int16_t);
    memcpy(&dinero, buffer, sizeof(int16_t));
    buffer+=sizeof(int16_t);
    memcpy(&indexPlayer, buffer, sizeof(int16_t)); 

    return 0;
}

ComprarCalleMsg::ComprarCalleMsg(std::string Nombre, int16_t IndexCasilla, int16_t BuyPrice):nombre(Nombre),indexCasilla(IndexCasilla),buyPrice(BuyPrice){
}

void ComprarCalleMsg::to_bin(){
    alloc_data(MESSAGE_SIZE);

    memset(_data, 0, MESSAGE_SIZE);
    //Serializar los campos type, nick y message en el buffer _data
    char* tmp = _data;
    memcpy(tmp, &type, sizeof(uint8_t));
    tmp += sizeof(uint8_t);
    
    memcpy(tmp, nombre.c_str(), sizeof(char)* 24);   
    tmp += sizeof(char) * 24;

    memcpy(tmp, &indexCasilla, sizeof(int16_t));
    tmp += sizeof(int16_t);

    memcpy(tmp, &buyPrice, sizeof(int16_t));
}
int ComprarCalleMsg::from_bin(char * bobj){
    alloc_data(MESSAGE_SIZE);
    memcpy(static_cast<void*>(_data), bobj, MESSAGE_SIZE); 
    //Reconstruir la clase usando el buffer _data
    char* buffer = _data;
    memcpy(&type, buffer, sizeof(uint8_t)); 
    buffer += sizeof(uint8_t);

    char nick_buffer[24]; 
    memcpy(&nick_buffer, buffer, 24 * sizeof(char)); 
    nombre = std::string(nick_buffer); 
    buffer += 24* sizeof(char);

    memcpy(&indexCasilla, buffer, sizeof(int16_t));
    buffer+=sizeof(int16_t);

    memcpy(&buyPrice, buffer, sizeof(int16_t));
    return 0;
}

PagarMsg::PagarMsg(int16_t BuyPrice):buyPrice(BuyPrice){
}
void PagarMsg::to_bin(){
    alloc_data(MESSAGE_SIZE);
    memset(_data, 0, MESSAGE_SIZE);
    //Serializar los campos type, nick y message en el buffer _data
    char* tmp = _data;
    memcpy(tmp, &type, sizeof(uint8_t));
    tmp += sizeof(uint8_t);
    memcpy(tmp, &buyPrice, sizeof(int16_t));
}
int PagarMsg::from_bin(char * bobj){
    alloc_data(MESSAGE_SIZE);
    memcpy(static_cast<void*>(_data), bobj, MESSAGE_SIZE); 
    //Reconstruir la clase usando el buffer _data
    char* buffer = _data;
    memcpy(&type, buffer, sizeof(uint8_t)); 
    buffer += sizeof(uint8_t);
    memcpy(&buyPrice, buffer, sizeof(int16_t));
    return 0;
}

CasaMsg::CasaMsg(int16_t IndexPosition,int16_t NumCasas):indexPosition(IndexPosition),numCasas(NumCasas),msgResponse(""){
    setType(CASA);
}
void CasaMsg::to_bin(){
    alloc_data(MESSAGE_SIZE);
    memset(_data, 0, MESSAGE_SIZE);
    //Serializar los campos type, nick y message en el buffer _data
    char* tmp = _data;
    memcpy(tmp, &type, sizeof(uint8_t));
    tmp += sizeof(uint8_t);
    memcpy(tmp, &indexPosition, sizeof(int16_t));
    tmp += sizeof(uint16_t);
    memcpy(tmp, &numCasas, sizeof(int16_t));
    tmp += sizeof(uint16_t);
    memcpy(tmp, msgResponse.c_str(), sizeof(char) * 64);
}
int CasaMsg::from_bin(char * bobj){
    alloc_data(MESSAGE_SIZE);
    memcpy(static_cast<void*>(_data), bobj, MESSAGE_SIZE); 
    //Reconstruir la clase usando el buffer _data
    char* buffer = _data;
    memcpy(&type, buffer, sizeof(uint8_t)); 
    buffer += sizeof(uint8_t);
    memcpy(&indexPosition, buffer, sizeof(int16_t));
    buffer += sizeof(int16_t);
    memcpy(&numCasas, buffer, sizeof(int16_t));
    buffer += sizeof(int16_t);
    char message_buffer[65] = {0};
    memcpy(message_buffer, buffer, 64 * sizeof(char)); 
    msgResponse = std::string(message_buffer); 
    return 0;
}
CarcelMsg::CarcelMsg(int16_t BuyPrice):buyPrice(BuyPrice){
}
void CarcelMsg::to_bin(){
    alloc_data(MESSAGE_SIZE);
    memset(_data, 0, MESSAGE_SIZE);
    //Serializar los campos type, nick y message en el buffer _data
    char* tmp = _data;
    memcpy(tmp, &type, sizeof(uint8_t));
    tmp += sizeof(uint8_t);
    memcpy(tmp, &buyPrice, sizeof(int16_t));
}
int CarcelMsg::from_bin(char * bobj){
    alloc_data(MESSAGE_SIZE);
    memcpy(static_cast<void*>(_data), bobj, MESSAGE_SIZE); 
    //Reconstruir la clase usando el buffer _data
    char* buffer = _data;
    memcpy(&type, buffer, sizeof(uint8_t)); 
    buffer += sizeof(uint8_t);
    memcpy(&buyPrice, buffer, sizeof(int16_t));
    return 0;
}

HipotecaMsg::HipotecaMsg(int16_t IndexPosition,bool Hipoteca):indexPosition(IndexPosition),hipoteca(Hipoteca){
    setType(HIPOTECA);
}

void HipotecaMsg::to_bin(){
    alloc_data(MESSAGE_SIZE);
    memset(_data, 0, MESSAGE_SIZE);
    //Serializar los campos type, nick y message en el buffer _data
    char* tmp = _data;
    memcpy(tmp, &type, sizeof(uint8_t));
    tmp += sizeof(uint8_t);
    memcpy(tmp, &indexPosition, sizeof(int16_t));
    tmp+=sizeof(uint16_t);
    memcpy(tmp, &hipoteca, sizeof(uint16_t));
    tmp+=sizeof(uint16_t);
    memcpy(tmp, msgResponse.c_str(), sizeof(char) * 48);
}
int HipotecaMsg::from_bin(char * bobj){
    alloc_data(MESSAGE_SIZE);
    memcpy(static_cast<void*>(_data), bobj, MESSAGE_SIZE); 
    //Reconstruir la clase usando el buffer _data
    char* buffer = _data;
    memcpy(&type, buffer, sizeof(uint8_t)); 
    buffer += sizeof(uint8_t);
    memcpy(&indexPosition, buffer, sizeof(int16_t));
    buffer+=sizeof(int16_t);
    memcpy(&hipoteca, buffer, sizeof(int16_t));
    buffer+=sizeof(int16_t);
    char message_buffer[49] = {0};
    memcpy(message_buffer, buffer, 48 * sizeof(char)); 
    msgResponse = std::string(message_buffer); 
    return 0;
}
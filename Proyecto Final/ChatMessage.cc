#include "ChatMessage.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void ChatMessage::to_bin(){
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

int ChatMessage::from_bin(char * bobj){
    alloc_data(MESSAGE_SIZE); 
    memcpy(static_cast<void*>(_data), bobj, MESSAGE_SIZE); 
    //Reconstruir la clase usando el buffer _data
    char* buffer = _data; memcpy(&type, buffer, sizeof(uint8_t)); 
    buffer += sizeof(uint8_t); char nick_buffer[8] = {0}; 
    memcpy(nick_buffer, buffer, 8* sizeof(char)); 
    nick = std::string(nick_buffer); 
    buffer += 8* sizeof(char); char message_buffer[81] = {0};
    memcpy(message_buffer, buffer, 80 * sizeof(char)); 
    message = std::string(message_buffer); 
    return 0;
}

void PlayerSerializable::to_bin(){
    alloc_data(MESSAGE_SIZE);

    memset(_data, 0, MESSAGE_SIZE);
    //Serializar los campos type, nick y message en el buffer _data
    char* tmp = _data;
    memcpy(tmp, nick.c_str(), sizeof(char)* 10);   
    tmp += sizeof(char) * 10;
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
    char nick_buffer[10]; 
    memcpy(&nick_buffer, buffer, 10* sizeof(char)); 
    nick = std::string(nick_buffer); 
    buffer += 10* sizeof(char);
    memcpy(&indexPosition, buffer, sizeof(int16_t));
    buffer+=sizeof(int16_t);
    memcpy(&dinero, buffer, sizeof(int16_t));
    buffer+=sizeof(int16_t);
    memcpy(&indexPlayer, buffer, sizeof(int16_t)); 

    return 0;
}
#include "Serializable.h"

#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

class Jugador: public Serializable  
{
public:
    Jugador(const char * _n, int16_t _x, int16_t _y):x(_x),y(_y)
    {
        strncpy(name, _n, MAX_NAME);
    };

    virtual ~Jugador(){};

    void to_bin(){
        size_t size = 2*sizeof(int16_t)+MAX_NAME*sizeof(char);
        alloc_data(size);
        char* tmp = _data;
        memcpy(_data,name,sizeof(char)*MAX_NAME);
        tmp+=sizeof(char)*MAX_NAME;
        memcpy(tmp,&x,sizeof(int16_t));
        tmp+=sizeof(int16_t);     
        memcpy(tmp,&y,sizeof(int16_t));

    }

    int from_bin(char * data){
        memcpy(name,data,sizeof(char)*MAX_NAME);
        data+=sizeof(char)*MAX_NAME;
        memcpy(&x,data,sizeof(int16_t));
        data+=sizeof(int16_t);
        memcpy(&y,data,sizeof(int16_t));    
        return 0;
    }

public:
static const size_t MAX_NAME=20;
    char name[MAX_NAME];

    int16_t x;
    int16_t y;
};

int main(int argc, char **argv)
{
    Jugador one_r("", 0, 0);
    Jugador one_w("Player_ONE", 123, 987);

    // 1. Serializar el objeto one_w
    one_w.to_bin();

    // 2. Escribir la serialización en un fichero
    int fd= open("./jugador.bin",O_CREAT|O_TRUNC|O_RDWR,0777);
    write(fd,one_w.data(),one_w.size());

    // 3. Leer el fichero
    close(fd);
    fd= open("./jugador.bin",O_RDONLY);
    char datos[one_w.size()];
    read(fd,datos,256);

    // 4. "Deserializar" en one_r
    //char* data;
    //data= one_w.data();
    if(one_r.from_bin(datos)==-1){
        std::cout<<"ERROR DESERIALIZING\n";
        exit(1);
    }
    // 5. Mostrar el contenido de one_r  
    std::cout<<one_r.name<<"\n";
    std::cout<<one_r.x<<" ";
    std::cout<<one_r.y<<"\n";
    close(fd);
    return 0;
}

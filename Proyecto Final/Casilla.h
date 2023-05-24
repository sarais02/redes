#pragma once

class Casilla{
    public:
    Casilla(){};
    //Enum para ver el tipo de casilla
    enum Type{Calle=0,Carcel,Suerte,Comunidad,Impuesto,Salida};
    //Este int representa a el jugador que posee esta casilla, si es -1 no es de nadie
    int Propiedad=-1;
    virtual void llegar(){};
    Type myType;
};
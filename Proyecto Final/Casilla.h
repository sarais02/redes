#ifndef CASILLA_H_
#define CASILLA_H_
#include <string>
#include <vector>

//Enum para ver el tipo de casilla
enum Type: u_int8_t{NOTHING=0,Calle=1,Carcel=2,Suerte=3,Comunidad=4,Impuesto=5,Salida=6};

class Casilla{
protected:
    //Nombre de la casilla
    std::string name="";   
    //Tipo de Casilla
    u_int8_t myType=NOTHING;
    
public:
    Casilla(){};
    Casilla(std::string namee, u_int8_t type);
    ~Casilla(){};
    
    inline u_int8_t getType(){return myType;};   

    //Cuando llegue un jugador a esa casilla que tiene que pasar
    virtual void getSomething(){};

};

//Tipo de casilla comprable, en la cual se pueden poner casas
class Calle: public Casilla{
    //Precio De Comprar La Calle
    int buyPrice;
    //Este int representa a el jugador que posee esta casilla, si es -1 no es de nadie
    int property=-1;
    //Dinero que te dan al hipotecar la calle
    int mortgage;
    //Indice de precios que el jugador que no es de propiedad de la calle debe pagar al caer en la casilla
    int indexRentPrices=0;
    //Precios que deben comerlos jugadores
    std::vector<int> rentPrices;

public:
    Calle(std::string namee, u_int8_t type, std::vector<int> prices,int mortgagee);
    ~Calle()=default;

    inline int getPrice(){return buyPrice;};
    inline int getProperty(){return property;};

    void getSomething() override;
};
#endif
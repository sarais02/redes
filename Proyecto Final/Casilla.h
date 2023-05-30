#ifndef CASILLA_H_
#define CASILLA_H_
#include <string>
#include <vector>

//Enum para ver el tipo de casilla
enum Type: u_int8_t{NOTHING=0,CALLE=1,CARCEL=2,SUERTE=3,COMUNIDAD=4,IMPUESTO=5,SALIDA=6,ESTACION=7,SERVICIOS=8};

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
    inline std::string getName(){return name;};
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
    //Conste de contruir una casa en la propiedad
    int housePrice;
    //CALLES DE COLOR
    std::vector<int> familyColors;
public:
    Calle(std::string namee, u_int8_t type, std::vector<int> prices,int buy,int mortgagee,int house,std::vector<int> familyColors);
    ~Calle()=default;

    inline int getPrice(){return buyPrice;};
    inline int getProperty(){return property;};
    inline int getHousePrice(){return housePrice;};
    inline void setProperty(int index){property=index;};    
    inline int rentCost(){return rentPrices[indexRentPrices];};
    inline std::vector<int> getFamily(){return familyColors;};
    void getSomething() override;
    void setRentIndex(int num);
};

class Impuesto: public Casilla{
    int16_t taxes;

public:
    Impuesto(std::string name,u_int8_t type, int16_t taxes_);
    ~Impuesto()=default;

    inline int16_t getPrice(){return taxes;};
};

class Salida: public Casilla{
    int16_t reward;

public:
    Salida(std::string name,u_int8_t type, int16_t reward_);
    ~Salida()=default;

    inline int16_t getReward(){return reward;};
};
#endif
#include "Casilla.h"

Casilla::Casilla(std::string namee, u_int8_t type):name(namee),myType(type){  
};

Calle::Calle(std::string namee, u_int8_t type, std::vector<int> prices,int mortgagee):Casilla(namee,type),rentPrices(prices),mortgage(mortgagee){
};

void Calle::getSomething(){
    
}
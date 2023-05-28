#include "Casilla.h"

Casilla::Casilla(std::string namee, u_int8_t type):name(namee),myType(type){  
};

Calle::Calle(std::string namee, u_int8_t type, std::vector<int> prices,int buy,int mortgagee,int house,std::vector<int> familyColors):
Casilla(namee,type),rentPrices(prices),mortgage(mortgagee),housePrice(house),buyPrice(buy){
    for (int i = 0; i < prices.size(); i++)
        rentPrices.push_back(prices[i]);
    for (int i = 0; i < familyColors.size(); i++)
        this->familyColors.push_back(familyColors[i]);
        
};

void Calle::getSomething(){
    
}
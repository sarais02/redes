#include "Casilla.h"
#include <iostream>
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

void Calle::setRentIndex(int num){
    if(num>rentPrices.size()){
        std::cout<<"CALLE CON EL NUMERO MAXIMO DE CASAS\n";
        num= rentPrices.size();
    }
    indexRentPrices=num;
    
}

Impuesto::Impuesto(std::string name_,u_int8_t type_, int16_t taxes_): Casilla(name_, type_), taxes(taxes_){
}
Salida::Salida(std::string name_,u_int8_t type_, int16_t reward_): Casilla(name_, type_), reward(reward_){
}
Carcel::Carcel(std::string name_,u_int8_t type_, int16_t taxes_): Casilla(name_, type_), taxes(taxes_){
}

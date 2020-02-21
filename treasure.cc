/* 
 * File:   treasure.cc
 * Author: Nathan Duma
 * 
 * Created on March 26, 2017, 8:48 PM
 */

#include <future>

#include "treasure.h"
#include "hero.h"

Treasure::Treasure(char type) {
    this->display = 'G';
    this->type = type;
    
    if (this->type == '6'){ // normal pile
        this->amount = 2;
    } else if (this->type == '7'){ // small hoard
        this->amount = 1;
    } else if (this->type == '8'){ // merchant hoard
        this->amount = 4;
    } else if (this->type == '9'){ // dragon hoard
        this->amount = 6;
    }
}

void Treasure::useItem(shared_ptr<Hero> c){
    c.get()->giveGold(this->amount);
}

char Treasure::getDisplay(){
    return this->display;
}

char Treasure::getType(){
    return this->type;
}

int Treasure::getAmount(){
    return this->amount;
}

Treasure::~Treasure() {
}


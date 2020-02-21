/* 
 * File:   treasure.h
 * Author: Nathan Duma
 *
 * Created on March 26, 2017, 8:48 PM
 */

#ifndef TREASURE_H
#define TREASURE_H

#include "gameobject.h"
#include "item.h"

class Character;

class Treasure: public Item {
    int amount;
public:
    Treasure(char type);
    void useItem(shared_ptr<Hero> c);
    virtual char getDisplay();
    char getType();
    int getAmount();
    virtual ~Treasure();
private:

};

#endif /* TREASURE_H */


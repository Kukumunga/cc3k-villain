#ifndef __POTION_H
#define __POTION_H

#include "item.h"
class Hero;
class Potion: public Item {
    
	
public:
    void useItem(shared_ptr<Hero> c);
    Potion(char type);
    char getDisplay();
    char getType();
    ~Potion();
private:

};

#endif /* POTION_H */


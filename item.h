#ifndef __ITEM_H
#define __ITEM_H

#include "gameobject.h"
#include <memory>

class Hero;

class Item: public GameObject{
protected:
	char type;
public:
	virtual void useItem(shared_ptr<Hero> c) = 0;
        virtual char getType() = 0;
	virtual char getDisplay() = 0;
};
#endif

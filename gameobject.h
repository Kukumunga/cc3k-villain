#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

class GameObject{
protected:
	int x;
        int y;
	char display;
public:
	int getX();
	int getY();
        void setX(int x);
        void setY(int y);
	virtual char getDisplay() = 0;
	virtual ~GameObject();

};
#endif
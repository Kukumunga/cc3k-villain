#ifndef __CHARACTER_H
#define __CHARACTER_H
#include <sstream>
#include <string>
#include "gameobject.h"
#include <math.h>
#include <memory>
using namespace std;

class Character: public GameObject{
protected:
    int hp;
    int atk;
    int def;
    int gold;
    char race;
    char display;
public:
    virtual int gethp() = 0;
    virtual int getatk() = 0;
    virtual int getdef() = 0;
    virtual void sethp(int h) = 0;
    virtual void setatk(int a) = 0;
    virtual void setdef(int d) = 0;
    virtual char getDisplay() = 0;
    virtual void attack(shared_ptr<Character> c) = 0;
    virtual char getRace() = 0;
    virtual int getGold() = 0;
    virtual ~Character(){}
};
#endif

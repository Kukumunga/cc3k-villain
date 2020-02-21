#ifndef __ENEMY_H__
#define __ENEMY_H__
#include <sstream>
#include <string>

#include "character.h"
#include <memory>

using namespace std;

class Enemy: public Character {
private:
    bool neutral;

public:
    void makeHostile();
    bool isHostile();
    Enemy(char race);
    int gethp();
    int getatk();
    int getdef();
    void sethp(int h);
    void setatk(int a);
    void setdef(int d);
    char getDisplay();
    int getGold();
    int dropGold();
    void attack(shared_ptr<Character> c);
    char getRace();
    ~Enemy(); //destructor
    //create something to accept things after compilation
};

#endif

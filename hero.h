/* 
 * File:   hero.h
 * Author: Nathan Duma
 *
 * Created on March 26, 2017, 7:56 PM
 */

#ifndef HERO_H
#define HERO_H

#include <string>
#include "character.h"

using namespace std;


class Hero: public Character {
    int basehp;
    int baseatk;
    int basedef;
public:
    Hero(char race);
    int gethp();
    int getatk();
    int getdef();
    int getbasehp();
    int getbaseatk();
    int getbasedef();
    void sethp(int h);
    void setatk(int a);
    void setdef(int d);
    void attack(shared_ptr<Character> c);
    char getRace();
    char getDisplay();
    int getGold(); //return total gold
    void giveGold(int amount);
    ~Hero();
private:

};

#endif /* HERO_H */


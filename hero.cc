/* 
 * File:   Hero.cc
 * Author: Nathan Duma
 * 
 * Created on March 26, 2017, 7:56 PM
 */

#include "hero.h"
#include "enemy.h"
#include <cstdlib>


Hero::Hero(char race){
    this->race = race;
    this->display = '@';
    this->gold = 0;
    //initializing Hero to shade character
    // do these have to be captials?
    if(this->race == 's'){
        this->hp = 125;
        this->basehp = 125;
        this->atk = 25;
        this->baseatk = 25;
        this->def = 25;
        this->basedef = 25;
    }
    //initializing Hero to drow character
    else if(this->race == 'd'){
        this->hp = 150;
        this->basehp = 150;
        this->atk = 25;
        this->baseatk = 25;
        this->def = 15;
        this->basedef = 15;
    }
    //initializing Hero to vampire character
    else if(this->race == 'v'){
        this->hp = 50;
        this->basehp = 50;
        this->atk = 25;
        this->baseatk = 25;
        this->def = 25;
        this->basedef = 25;
    }
    //initializing Hero to goblin character
    else if(this->race == 'g'){
        this->hp = 110;
        this->basehp = 110;
        this->atk = 15;
        this->baseatk = 15;
        this->def = 20;
        this->basedef = 20;
    }
    //initializing Hero to troll character
    else if (this->race == 't'){
        this->hp = 120;
        this->basehp = 120;
        this->atk = 25;
        this->baseatk = 25;
        this->def = 15;
        this->basedef = 15;
    } else if (this->race == 'z'){
        this->hp = 9999;
        this->basehp = 9999;
        this->atk = 100;
        this->baseatk = 100;
        this->def = 100;
        this->basedef = 100;
    } else if (this->race == 'r'){
        this->hp = 1337;
        this->basehp = 1337;
        this->atk = 1337;
        this->baseatk = 1337;
        this->def = 1337;
        this->basedef = 1337;
    }
}
int Hero::gethp(){
    return this->hp;
}

int Hero::getatk(){
    return this->atk;
}

int Hero::getdef(){
    return this->def;
}

void Hero::sethp(int h){
    char vampire = 'V';
    if (this->getRace() != vampire && h > this->getbasehp()){ // vampires have no base hp
        this->hp = this->getbasehp();
    } else if (h < 0){
        this->hp = 0;
    }else{
        this->hp = h;
    }
}
void Hero::setatk(int a){
    this->atk = a;
}
void Hero::setdef(int d){
    this->def = d;
}

static double max(double a, double b){
    return (a < b ? b : a);
}

void Hero::attack(shared_ptr<Character> c){
    shared_ptr<Enemy> e = dynamic_pointer_cast<Enemy>(c);
    // attack the enemy
    double minhp = 0;
    double hp = e.get()->gethp() - ceil((100.0/(100.0+e.get()->getdef())) * this->getatk());
    bool attacked = false;
    
    int num = rand() % 2;
    char halfling = 'L';
    char vampire = 'V';
    
    if (e.get()->getDisplay() != halfling || // any enemy will get attacked at 100% rate except:
       (e.get()->getDisplay() == halfling && num == 0)){ // 50% chance to attack halfling
        e.get()->sethp((int)max(hp, minhp));
        attacked = true;
    }
    
    const char dwarf = 'W';
    
    if (attacked == true && this->getRace() == vampire &&
        e.get()->getRace() != dwarf){ // vampire gets 5 hp for successful attack
        int gain = 5;
        this->sethp(this->gethp() + gain);
    } else if (attacked = true && this->getRace() == vampire){
        int lose = 5;
        this->sethp(this->gethp() - lose);
    }  
}

char Hero::getDisplay(){
    return this->display;
}
int Hero::getbasehp(){
    return this->basehp;
}
int Hero::getbaseatk(){
    return this->baseatk;
}
int Hero::getbasedef(){
    return this->basedef;
}


int Hero::getGold(){
    return this->gold;
}
void Hero::giveGold(int amount){
    this->gold += amount;
    cout << "gold: " << this->gold << endl;
}


char Hero::getRace(){
    return this->race;
}

Hero::~Hero(){
    
}



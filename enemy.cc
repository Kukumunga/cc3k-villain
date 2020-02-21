/* 
 * File:   enemy.cc
 * Author: Nathan Duma
 * 
 * Created on March 26, 2017, 7:56 PM
 */

#include "enemy.h"
#include "hero.h"
#include <cstdlib>


Enemy::Enemy(char race){
    this->neutral = false;
    this->race = race;
    this->display = race;
    //initializing enemy human character
    if(this->race == 'H'){
        this->hp = 140;
        this->atk = 20;
        this->def = 20;
        this->gold = 4;
    }
    //initializing enemy dwarf character
    else if(this->race == 'W'){
        this->hp = 100;
        this->atk = 20;
        this->def = 30;
       
    }
    //initializing enemy elf character
    else if(this->race == 'E'){
        this->hp = 140;
        this->atk = 30;
        this->def = 10;
       
    }
    //initializing enemy orc character
    else if(this->race == 'O'){
        this->hp = 180;
        this->atk = 30;
        this->def = 25;
       // this->gold = ;
    }
    //initializing enemy merchant character
    else if(this->race == 'M'){
        this->hp = 30;
        this->atk = 70;
        this->def = 5;
        this->gold = 4;
        this->neutral = true;  
    }
    else if (this->race == 'L'){
        this->hp = 100;
        this->atk = 15;
        this->def = 20;
    }
    else if (this->race == 'D'){
        this->hp = 150;
        this->atk = 20;
        this->def = 20; 
    }

}

int Enemy::gethp(){
    return this->hp;
}

int Enemy::getatk(){
    return this->atk;
}

int Enemy::getdef(){
    return this->def;
}

void Enemy::sethp(int h){
    if (h < 0){
        this->hp = 0;
    } else{
        this->hp = h;
    }
}
void Enemy::setatk(int a){
    this->atk = a;
}
void Enemy::setdef(int d){
    this->def = d;
}

static double max(double a, double b){
    return (a < b ? b : a);
}

void Enemy::attack(shared_ptr<Character> c){
    // this function is used to attack the hero that moved around the enemy
    // this attack function is used when the hero steps on the square
    // initially, and so the enemy automatically attacks and the hero
    // does not attack
    shared_ptr<Hero> h = dynamic_pointer_cast<Hero>(c);
    double minhp = 0;
    const char goblin = 'g';
    double hp;
    
    if (h.get()->getRace() == goblin){ // 50% more damage to goblins
        hp = h.get()->gethp() - (2.0 * ceil((100.0/(100.0+h.get()->getdef())) * this->getatk()));
    } else{
        hp = h.get()->gethp() - ceil((100.0/(100.0+h.get()->getdef())) * this->getatk());
    }
    
    int num = rand() % 2;
    const char drow = 'd';
    const char elf = 'E';

    
    if (!this->neutral){
        if (num == 0){
            h.get()->sethp((int)max(hp, minhp));
        }
        num = rand() % 2;
        if (num == 0 && h.get()->getRace() != drow && this->getRace() == elf){
            h.get()->sethp((int)max(hp, minhp));
        }
    }
    h.reset();
}

void Enemy::makeHostile(){
    this->neutral = false;
}

bool Enemy::isHostile(){
    return !this->neutral;
}

char Enemy::getDisplay(){
    return this->display;
}


char Enemy::getRace(){
    return this->race;
}

int Enemy::dropGold(){
    return this->gold;
}
int Enemy::getGold(){
    return 0;
}
Enemy::~Enemy(){
    
}


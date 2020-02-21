#include "gameobject.h"

int GameObject::getX(){
    return x;
}

int GameObject::getY(){
    return y;
}

void GameObject::setX(int x){
    this->x = x;
}

void GameObject::setY(int y){
    this->y = y;
}

char GameObject::getDisplay(){
    return display;
}

GameObject::~GameObject(){
    
}


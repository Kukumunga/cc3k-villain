#include <utility>
#include "subject.h"
#include "observer.h"
#include "cell.h"
#include "celltype.h"
#include "info.h"
#include "character.h"

Cell::Cell() {
}

Cell::~Cell(){
}


Info Cell::getInfo() const {
    Info i{this->x, this->y, this->t, this->o.get()};
    return i;
}


void Cell::setup(int x, int y, char c, shared_ptr<GameObject> o) {
    this->x = x;
    this->y = y;
    this->t = this->findType(c);
    this->o = o;
}

void Cell::notify(Subject &whoNotified) {
    this->notifyObservers(CellType::TILE);
}

CellType Cell::findType(char display){
    if (display == '\\'){
        return CellType::STAIR;
    } else if (display == '#'){
        return CellType::PASSAGE;
    } else if (display == '+'){
        return CellType::DOOR;
    } else if (display == '.'){
        return CellType::TILE;
    } else if (display == '|'){
        return CellType::VWALL;
    } else if (display == '-'){
        return CellType::HWALL;
    } else if (display == ' '){
        return CellType::NONE;
    } else{
        // it's an object that sits on a TILE
        return CellType::TILE;
    }
}

CellType Cell::subType() const {
    this->t; 
}


#include "subject.h"
#include "observer.h"
#include "celltype.h"
#include <iostream>
#include <memory>

using namespace std;

void Subject::attach(Observer *o) {
    this->observers.emplace_back(o); // pushes the observer onto the observer this
}

void Subject::notifyObservers(CellType t) {
    for (int i = 0; i < this->observers.size(); i++) {
        if (t == this->observers[i]->subType()){ // if the subtype of the observer matches
            this->observers[i]->notify(*this); // then notify the relevant observers
        } 
    }
}

Subject::~Subject(){
    observers.clear();
}




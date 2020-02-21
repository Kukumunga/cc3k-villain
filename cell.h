#ifndef __CELL_H__
#define __CELL_H__
#include <iostream>
#include <sstream>
#include <string>
#include "celltype.h"
#include "gameobject.h"
#include "subject.h"
#include "textdisplay.h"
#include "observer.h"
#include <memory>

using namespace std;
class Info;

class Cell: public Subject, public Observer{
	int x;
        int y;
        CellType t;
        
public:
  shared_ptr<GameObject> o;
  Cell();  // Default constructor
  ~Cell(); // dtor

  void setup(int x, int y, char c, shared_ptr<GameObject> o); // sets up the cells

  CellType subType() const override;
  void notify(Subject &whoNotified); // this will be called so all the observers
                                     // can change their display properly
  CellType findType(char c);
  Info getInfo() const override;
	
};
#endif
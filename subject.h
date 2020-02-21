#ifndef _SUBJECT_H_
#define _SUBJECT_H_
#include <vector>
#include "celltype.h"
#include <memory>

class Info;
class Observer;
using namespace std;

class Subject {
  vector<Observer*> observers;
 public:
  void attach(Observer *o);  
  void notifyObservers(CellType t);
  virtual Info getInfo() const = 0;
  virtual ~Subject() = 0;
};

#endif



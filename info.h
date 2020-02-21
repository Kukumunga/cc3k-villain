/* 
 * File:   info.h
 * Author: Nathan Duma
 *
 * Created on March 18, 2017, 4:41 PM
 */

#ifndef __INFO_H__
#define __INFO_H__

// do you have to include these?
#include "celltype.h"
#include "gameobject.h"
#include <memory>

// maybe you have to include these?
enum CellType;
class GameObject;

struct Info {
  int x; 
  int y;
  CellType t;
  GameObject *o; // this might cause a memory leak but oh well
};

#endif



#ifndef __TEXTDISPLAY_H__
#define __TEXTDISPLAY_H__
#include <iostream>
#include <vector>
#include "observer.h"
class Cell;
using namespace std;

class TextDisplay: public Observer {
  vector<vector<char>> theDisplay;
  const int height = 25; // The height of the board
  const int width = 79;    // The width of the board
  int floor; // TODO: impliment the different floors

 public:
  TextDisplay(vector<vector<char>> board);

  void notify(Subject &whoNotified) override;
  CellType subType() const override;

  ~TextDisplay();

  friend std::ostream &operator<<(ostream &out, const TextDisplay &td);
};

#endif


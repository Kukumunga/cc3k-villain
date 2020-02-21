/* 
 * File:   board.h
 * Author: Nathan Duma
 *
 * Created on March 26, 2017, 10:07 AM
 */

#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include "cell.h"
#include "textdisplay.h"
#include "gameobject.h"
#include "character.h"
#include "potion.h"
#include "treasure.h"
#include "enemy.h"
#include "hero.h"
#include <map>
#include <memory>

using namespace std;

class Board {
  vector<vector<Cell>> board;  // The actual board.
  const int height = 25; // The height of the board
  const int width = 79;    // The width of the board
  unique_ptr<TextDisplay> td;
  int floor;
  bool random;
  bool hostile;
  const string enemy = "HWEOMDL";
  map<char,string> potmap = {{'0', "RH"}, {'1', "BA"}, {'2', "BD"},
                             {'3', "PH"}, {'4', "WA"}, {'5', "WD"}};
  map<string,string> dir = {{"no", "North"}, {"so", "South"}, {"ea", "East"},
                             {"we", "West"}, {"ne", "North-East"}, 
                             {"nw", "North-West"}, {"se", "South-East"},
                             {"sw", "South-West"}};
  map<char,string> enemymap = {{'H', "Human"}, {'W', "Dwarf"},
                             {'O', "Orc"}, {'M', "Merchant"}, 
                             {'D', "Dragon"}, {'L', "Halfling"},
                             {'E', "Elf"}};
  const string potionNum = "012345";
  const string treasure = "6789";
  const string hero = "@";
  const char gold = 'G';
  const char potion = 'P';
  const char merchant = 'M';
  const char dragon = 'D';
  const char human = 'H';
  const char troll = 't';
  const char goblin = 'g';
  const char humanhoard = '6';
  const char smallhoard = '7';
  const char merchanthoard = '8';
  const char dragonhoard = '9';

  void init(vector<vector<char>> board, shared_ptr<Character> c, int floor, bool hostile); // initalize the board
  void notifyMerchants();
  vector<vector<char>> makeRandom(vector<vector<char>> b); // makes the board have random stuff on it
 public:
  Board(vector<vector<char>> board, shared_ptr<Character> c, int floor, bool random, bool hostile); // Sets up the board.
  ~Board();
  int getFloor();
  string move(shared_ptr<Character> c, string dir);
  string usePotion(shared_ptr<Character> c, string dir);
  string attack(shared_ptr<Character> c, string dir);
  string computeNextIteration(shared_ptr<Character> c);
  bool getHostile();
  friend ostream &operator<<(ostream &out, const Board &b);
  
};


#endif



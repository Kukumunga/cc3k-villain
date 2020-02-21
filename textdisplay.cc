/* 
 * Author: Nathan Duma
 * 
 * Created on March 18, 2017, 4:44 PM
 */

#include <iostream>
#include <utility>
#include "textdisplay.h"
#include "cell.h"
#include "info.h"
using namespace std;

TextDisplay::TextDisplay(vector<vector<char>> board){
    string potion = "012345";
    string treasure = "6789";
    for (int i = 0; i < this->height; i++) {
        // push a vector of characters into the vector theDisplay
        this->theDisplay.push_back(vector<char>());
        // we need to resize the vector so we can fit each line
        this->theDisplay[i].resize(this->width);
        for (int k = 0; k < this->width; k++) {
            if (potion.find(board[i][k]) != -1){ // if there is a potion
                this->theDisplay[i][k] = 'P'; // hide it using a potion
            } else if (treasure.find(board[i][k]) != -1){ // if it's a treasure
                this->theDisplay[i][k] = 'G'; // hide it using gold
            } else{
                this->theDisplay[i][k] = board[i][k];
            }
        }
    }
}

void TextDisplay::notify(Subject &whoNotified) {
    // get rows and cols so we can put them in nicley
    int x = whoNotified.getInfo().x;
    int y = whoNotified.getInfo().y;
    char c;
    if (whoNotified.getInfo().o != nullptr){
        c = whoNotified.getInfo().o->getDisplay();
    } else{
        if (whoNotified.getInfo().t == CellType::STAIR){
            c = '\\';
        } else if (whoNotified.getInfo().t == CellType::PASSAGE){
            c = '#';
        } else if (whoNotified.getInfo().t == CellType::DOOR){
            c = '+';
        } else if (whoNotified.getInfo().t == CellType::TILE){
            c = '.';
        } else if (whoNotified.getInfo().t == CellType::VWALL){
            c = '|';
        } else if (whoNotified.getInfo().t == CellType::HWALL){
            c = '-';
        } else if (whoNotified.getInfo().t == CellType::NONE){
            c = ' ';
        } else{
            cout << "there is some error" << endl;
    }
    }
    
    this->theDisplay[x][y] = c;
}

TextDisplay::~TextDisplay(){
    
}

CellType TextDisplay::subType() const {
  return CellType::TILE;
}

ostream &operator<<(ostream &out, const TextDisplay &td) {
    for (int i = 0; i < td.height; i++) {
        for (int k = 0; k < td.width; k++) {
            out << td.theDisplay[i][k]; // print each character at a time
        }
        out << endl; // this indicates each line is seperated
    }
  return out;
}


/* 
 * File:   board.cc
 * Author: Nathan Duma
 * 
 * Created on March 26, 2017, 10:07 AM
 */

#include "board.h"
#include "info.h"
#include <iostream>
#include <string>
// needed for random number generation
#include <cstdlib>
#include <ctime>
#include <utility>
#include <algorithm>
#include <future>

using namespace std;

Board::Board(vector<vector<char>> board, shared_ptr<Character> c, int floor, bool random, bool hostile){
    this->floor = floor;
    this->hostile = hostile;
    this->random = random;
    if (random){
        vector<vector<char>> randomboard = makeRandom(board); // makes the board have random stuff on it
        init (randomboard, c, floor, hostile);
    }else{
        init(board, c, floor, hostile);
    }

}

Board::~Board() {
    
}

void Board::init(vector<vector<char>> board, shared_ptr<Character> c, int floor, bool hostile){
    for (int i = 0; i < this->height; i++) {
        // first we push a vector of cell objects hright times
        // then we need to resize the vector so it contains width cells
        this->board.push_back(vector<Cell>());
        this->board[i].resize(this->width);
        for (int j = 0; j < this->width; j++) {
            // we have to initalize the cells with the proper 
            // cell type and we need to attach a gameobject where nescessary
            shared_ptr<GameObject> o;
            
            //populate the gameobject on the board
            if (potionNum.find(board[i][j]) != -1){ // if it's a potion,
                o = static_pointer_cast<Potion>(make_shared<Potion>(board[i][j]));
            } else if (treasure.find(board[i][j]) != -1){ // if it's a treasure
                o = static_pointer_cast<Treasure>(make_shared<Treasure>(Treasure(board[i][j])));
            } else if (hero.find(board[i][j]) != -1){ // if it's a hero
                o = c; 
            } else if (enemy.find(board[i][j]) != -1){ // if it's an enemy
                o = static_pointer_cast<Enemy>(make_shared<Enemy>(Enemy(board[i][j])));
            } else{
                o = nullptr;
            }
            
            // set x and y
            if (potionNum.find(board[i][j]) != -1 || treasure.find(board[i][j]) != -1
                    || hero.find(board[i][j]) != -1 || enemy.find(board[i][j])  != -1){
                o.get()->setX(i);
                o.get()->setY(j);
            }
            this->board[i][j].setup(i, j, board[i][j], o);
        }
    }
    
    td.reset(new TextDisplay(board));
    
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            // we will attach each grid point to td and then attach the tile neighbours
            this->board[i][j].attach(td.get());
            
            if (this->board[i][j].getInfo().t == CellType::TILE){
                bool bottom = false;
                bool top = false;
                bool left = false;
                bool right = false;
                
                // we want to attach all the walkable tiles together

                // definitely has a bottom neighbour
                if (i != this->height-1 &&
                        this->board[i+1][j].getInfo().t == CellType::TILE){
                    this->board[i][j].attach(&this->board[i+1][j]);
                    bottom = true;
                }
                // definitely has a top neighbour
                if (i != 0 &&
                        this->board[i-1][j].getInfo().t == CellType::TILE){
                    this->board[i][j].attach(&this->board[i-1][j]);
                    top = true;
                }
                // definitely has a right neighbour
                if (j != this->width-1 &&
                        this->board[i][j+1].getInfo().t == CellType::TILE){
                    this->board[i][j].attach(&this->board[i][j+1]);
                    right = true;
                }
                 // definitely has a left neighbour
                if (j != 0 &&
                        this->board[i][j-1].getInfo().t == CellType::TILE){
                    this->board[i][j].attach(&this->board[i][j-1]);
                    left = true;
                }

                // attach the corners if possible

                // definitely has a top left neighbour
                if (left && top &&
                        this->board[i-1][j-1].getInfo().t == CellType::TILE){
                    this->board[i][j].attach(&this->board[i-1][j-1]);
                }
                // definitely has a top right neighbour
                if (right && top &&
                        this->board[i+1][j-1].getInfo().t == CellType::TILE){
                    this->board[i][j].attach(&this->board[i+1][j-1]);
                }
                // definitely has a bottom left neighbour
                if (left && bottom &&
                        this->board[i-1][j+1].getInfo().t == CellType::TILE){
                    this->board[i][j].attach(&this->board[i-1][j+1]);
                }
                // definitely has a bottom right neighbour
                if (right && bottom &&
                        this->board[i+1][j+1].getInfo().t == CellType::TILE){
                    this->board[i][j].attach(&this->board[i+1][j+1]);
                }
            }
        }
    }
    
    if (this->hostile){
        this->notifyMerchants();
    }
}

static string to_str(int num){
    ostringstream s;
    s << num;
    return s.str();
}

int Board::getFloor(){
    return this->floor;
}

bool Board::getHostile(){
    return this->hostile;
}


static vector<int> changeDir(int i, int j, string dir){
    vector<int> newDir;
    if (dir == "no"){ // north only
        i--;
    } else if (dir == "so"){ // south only
        i++;
    } else if (dir == "ea"){ // easy only
        j++;
    } else if (dir == "we"){ // west only
        j--;
    } else if (dir == "ne"){ // north east
        i--;
        j++;
    } else if (dir == "nw"){ // north west
        i--;
        j--;
    } else if (dir == "se"){ // south east
        i++;
        j++;
    } else if (dir == "sw"){ // south west
        i++;
        j--;
    }
    newDir.push_back(i);
    newDir.push_back(j);
    return newDir;
}


string Board::move(shared_ptr<Character> c, string dir){
    
    string action;
    
    // get the position of the character so we can move accordingly
    int old_i = c.get()->getX();
    int old_j = c.get()->getY();
    
    vector<int> newDir = changeDir(old_i, old_j, dir);
    int i = newDir[0];
    int j = newDir[1];
    
    if (board[i][j].getInfo().t == CellType::NONE ||
        board[i][j].getInfo().t == CellType::HWALL ||
        board[i][j].getInfo().t == CellType::VWALL){
        return "Invalid move!";
        
    } else if (board[i][j].getInfo().t == CellType::TILE){ // it's a tile

        // check to see if there is treasure on it, if so, collect it
        if (board[i][j].o.get() != nullptr &&
            board[i][j].o.get()->getDisplay() == gold){ // if the cell has gold
            shared_ptr<Treasure> t = dynamic_pointer_cast<Treasure>(board[i][j].o);
            // first we need to check if there is a dragon around the gold
            if (t.get() && t.get()->getType() == dragonhoard){ // if the gold is a dragon hoard
                for (int a = i-1; a <= i+1; a++) {
                    for (int b = j-1; b <= j+1; b++) {
                        if (board[a][b].o.get() != nullptr &&
                            board[a][b].o.get()->getDisplay() == dragon){ // if there is a dragon
                            action += "You're too greedy, kill the dragon first.";
                            return action;
                    }
                    }
                }
                // if it didn't return, then we can pick up the gold.
                shared_ptr<Hero> h = dynamic_pointer_cast<Hero>(c);
                if (t){
                    t.get()->useItem(h); // give the gold
                    action += "PC picked up " + to_str(t.get()->getAmount()) + " gold. ";
                } else{
                    action += "something went wrong giving treasure";
                }
                h.reset();
                t.reset();
                board[i][j].o.reset();
            } else{ // it isn't a dragon hoard
                shared_ptr<Hero> h = dynamic_pointer_cast<Hero>(c);
                if (t){
                    t.get()->useItem(h); // give the gold
                    action += "PC picked up " + to_str(t.get()->getAmount()) + " gold.";
                } else{
                    action += "something went wrong giving treasure ";
                }
                // might need these
                h.reset();
                t.reset();
                board[i][j].o.reset();
            }
        } else if (board[i][j].o.get() != nullptr){
            return "Invalid move!";
        }
    }
    if (board[i][j].getInfo().t == CellType::STAIR){
        this->floor++;
    } else{
        // we are going to swap the objects and notify the textdisplay to change
        board[i][j].o.swap(board[old_i][old_j].o);
        board[i][j].notifyObservers(CellType::TILE);
        board[old_i][old_j].notifyObservers(CellType::TILE);

        // update the character movement
        c.get()->setX(i);
        c.get()->setY(j);
    }
    
    // checks for enemies / items
    if (action.length() == 0){
        action += "PC moves " + this->dir[dir];
    } else{
        action += "PC moves " + this->dir[dir];
    }
    
    string subaction;
    
    for (int a = i-1; a <= i+1; a++) {
        for (int b = j - 1; b <= j+1; b++) {
            if (board[a][b].o.get() != nullptr &&
                board[a][b].o.get()->getDisplay() == potion){
                subaction += "an unknown potion, ";
            } else if (board[a][b].o.get() != nullptr &&
                board[a][b].o.get()->getDisplay() == gold){
                subaction += "an unknown gold hoard, ";
            }
        }
    }
    if (subaction.length() != 0){
        action += " and sees: " + subaction.substr(0, subaction.length() - 2) + ". ";
    } else{
        action += " and sees nothing. ";
    }
    
    // every turn the troll character gets +5 hp
    if (c.get()->getRace() == troll){
        int gain = 5;
        c.get()->sethp(c.get()->gethp() + gain);
    }
    return action;
}


string Board::usePotion(shared_ptr<Character> c, string dir){
    string action;
    // get the position of the character so we can move accordingly
    int old_i = c.get()->getX();
    int old_j = c.get()->getY();
    vector<int> newDir = changeDir(old_i, old_j, dir);
    int i = newDir[0];
    int j = newDir[1];

    if (board[i][j].o.get() != nullptr &&
        board[i][j].o.get()->getDisplay() == potion){ // if the cell has a potion
        shared_ptr<Potion> p = dynamic_pointer_cast<Potion>(board[i][j].o);
        shared_ptr<Hero> h = dynamic_pointer_cast<Hero>(c);
        if (p && h){
            p.get()->useItem(h); // use the potion

            action += "PC uses " + this->potmap[p->getType()] + ".";
        } else{
            action += "something went wrong using potion";
        }
        // might need these
        h.reset();
        p.reset();
        board[i][j].o.reset();
        // update the display
        board[i][j].notifyObservers(CellType::TILE);
        board[old_i][old_j].notifyObservers(CellType::TILE);
        
        return action; // the potion has been used
    }
    return "PC tried to use a potion that was not there!"; // the potion has not been used, return empty string
}

void Board::notifyMerchants(){
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            if (this->board[i][j].o.get() != nullptr &&
                this->board[i][j].o.get()->getDisplay() == merchant){
                shared_ptr<Enemy> e = dynamic_pointer_cast<Enemy>(board[i][j].o);
                if (e){
                    e.get()->makeHostile();
                } else{
                    cout << "something went wrong making merchants hostile" << endl;
                }
            }
        }
    }
}



string Board::attack(shared_ptr<Character> c, string dir){
    string action;
    
    // get the position of the character
    int old_i = c.get()->getX();
    int old_j = c.get()->getY();
    
    vector<int> newDir = changeDir(old_i, old_j, dir);
    int i = newDir[0];
    int j = newDir[1];
    
    if (board[i][j].o.get() != nullptr &&
        enemy.find(board[i][j].o.get()->getDisplay()) != -1){ // if the cell has an enemy
        shared_ptr<Enemy> e = dynamic_pointer_cast<Enemy>(board[i][j].o);
        
        if (e){
            int hp_before = e.get()->gethp();
            c.get()->attack(e); // hero attacks enemy
            int hp_after = e.get()->gethp();
            action += "PC deals " + to_str(hp_before - hp_after) + " damage to " +
                    e.get()->getDisplay() + " (" + to_str(hp_after) + " HP).";
            if (board[i][j].o.get()->getDisplay() == merchant){ // notify all merchants to become hostile
                this->notifyMerchants();
                this->hostile = true;
            }
        } else{
            action += "something went wrong using attacking";
        }
        // might need these
        if (e.get()->gethp() <= 0){ // the enemy died
            char enemyrace = e.get()->getRace();
            int num = rand() % 2;
            e.reset();
            if (enemyrace == merchant){
                board[i][j].o.reset(new Treasure(merchanthoard)); // put treasure inside
            } else if (enemyrace == human){ // enemies drop 4/2 gold at 50/50
                if (num == 0){
                    board[i][j].o.reset(new Treasure(merchanthoard)); // 4 gold
                } else{
                    board[i][j].o.reset(new Treasure(humanhoard)); // 2 gold
                }
            } else if (enemyrace != dragon){ // all other enimies drop normal or small hoard at 50/50 except dragons
                if (num == 0){
                    board[i][j].o.reset(new Treasure(humanhoard)); // 2 gold
                } else{
                    board[i][j].o.reset(new Treasure(smallhoard)); // 1 gold
                }
            } else{
                board[i][j].o.reset();
            }
            if (board[i][j].o.get() != nullptr){
                board[i][j].o.get()->setX(i);
                board[i][j].o.get()->setY(j);
            }

            if (c.get()->getRace() == goblin){ // the goblin steals 5 gold from every enemy
                shared_ptr<Hero> h = dynamic_pointer_cast<Hero>(board[i][j].o);
                int goblinSteal = 5;
                h.get()->giveGold(goblinSteal);
            }
            // update the display
            board[i][j].notifyObservers(CellType::TILE);
            board[old_i][old_j].notifyObservers(CellType::TILE);
        }
        return action; // the attack  has been successful, filled string
    }
    
    return "PC tried to attack an enemy that's not there!"; // the attack was not successful, empty string

}

vector<vector<char>> Board::makeRandom(vector<vector<char>> b){
    
    vector<vector<char>> board = b;
    
     vector<pair<int,int>> chamber1;
     vector<pair<int,int>> chamber2;
     vector<pair<int,int>> chamber3;
     vector<pair<int,int>> chamber4;
     vector<pair<int,int>> chamber5;
    // put the random enemies on the board
    // refer to the probabilities
    // TODO
    chamber1 = {make_pair(3, 3), make_pair(3, 4), make_pair(3, 5), make_pair(3, 6), make_pair(3, 7), make_pair(3, 8), make_pair(3, 9), make_pair(3, 10), make_pair(3, 12), make_pair(3, 13), make_pair(3, 14), make_pair(3, 15), make_pair(3, 16), make_pair(3, 17), make_pair(3, 18), make_pair(3, 19), make_pair(3, 20), make_pair(3, 21), make_pair(3, 22), make_pair(3, 23), make_pair(3, 24), make_pair(3, 25), make_pair(3, 26), make_pair(3, 27), make_pair(3, 28), 
make_pair(4, 3), make_pair(4, 4), make_pair(4, 5), make_pair(4, 6), make_pair(4, 7), make_pair(4, 8), make_pair(4, 9), make_pair(4, 10), make_pair(4, 11), make_pair(4, 12), make_pair(4, 13), make_pair(4, 14), make_pair(4, 15), make_pair(4, 16), make_pair(4, 17), make_pair(4, 18), make_pair(4, 19), make_pair(4, 20), make_pair(4, 21), make_pair(4, 22), make_pair(4, 23), make_pair(4, 24), make_pair(4, 25), make_pair(4, 26), make_pair(4, 27), make_pair(4, 28), 
make_pair(5, 3), make_pair(5, 4), make_pair(5, 5), make_pair(5, 6), make_pair(5, 7), make_pair(5, 8), make_pair(5, 9), make_pair(5, 10), make_pair(5, 11), make_pair(5, 12), make_pair(5, 13), make_pair(5, 14), make_pair(5, 15), make_pair(5, 16), make_pair(5, 17), make_pair(5, 18), make_pair(5, 19), make_pair(5, 20), make_pair(5, 21), make_pair(5, 22), make_pair(5, 23), make_pair(5, 24), make_pair(5, 25), make_pair(5, 26), make_pair(5, 27), make_pair(5, 28), 
make_pair(6, 3), make_pair(6, 4), make_pair(6, 5), make_pair(6, 6), make_pair(6, 7), make_pair(6, 8), make_pair(6, 9), make_pair(6, 10), make_pair(6, 11), make_pair(6, 12), make_pair(6, 13), make_pair(6, 14), make_pair(6, 15), make_pair(6, 16), make_pair(6, 17), make_pair(6, 18), make_pair(6, 19), make_pair(6, 20), make_pair(6, 21), make_pair(6, 22), make_pair(6, 23), make_pair(6, 24), make_pair(6, 25), make_pair(6, 26), make_pair(6, 27), make_pair(6, 28)
};
    chamber2 = {make_pair(15, 4), make_pair(15, 5), make_pair(15, 6), make_pair(15, 7), make_pair(15, 8), make_pair(15, 9), make_pair(15, 10), make_pair(15, 11), make_pair(15, 12), make_pair(15, 13), make_pair(15, 14), make_pair(15, 15), make_pair(15, 16), make_pair(15, 17), make_pair(15, 18), make_pair(15, 19), make_pair(15, 20), make_pair(15, 21), make_pair(15, 22), make_pair(15, 23), make_pair(15, 24), 
make_pair(16, 4), make_pair(16, 5), make_pair(16, 6), make_pair(16, 7), make_pair(16, 8), make_pair(16, 9), make_pair(16, 10), make_pair(16, 11), make_pair(16, 12), make_pair(16, 13), make_pair(16, 14), make_pair(16, 15), make_pair(16, 16), make_pair(16, 17), make_pair(16, 18), make_pair(16, 19), make_pair(16, 20), make_pair(16, 21), make_pair(16, 22), make_pair(16, 23), make_pair(16, 24), 
make_pair(17, 4), make_pair(17, 5), make_pair(17, 6), make_pair(17, 7), make_pair(17, 8), make_pair(17, 9), make_pair(17, 10), make_pair(17, 11), make_pair(17, 12), make_pair(17, 13), make_pair(17, 14), make_pair(17, 15), make_pair(17, 16), make_pair(17, 17), make_pair(17, 18), make_pair(17, 19), make_pair(17, 20), make_pair(17, 21), make_pair(17, 22), make_pair(17, 23), make_pair(17, 24), 
make_pair(18, 4), make_pair(18, 5), make_pair(18, 6), make_pair(18, 7), make_pair(18, 8), make_pair(18, 9), make_pair(18, 10), make_pair(18, 11), make_pair(18, 12), make_pair(18, 13), make_pair(18, 14), make_pair(18, 15), make_pair(18, 16), make_pair(18, 17), make_pair(18, 18), make_pair(18, 19), make_pair(18, 20), make_pair(18, 21), make_pair(18, 22), make_pair(18, 23), make_pair(18, 24), 
make_pair(19, 4), make_pair(19, 5), make_pair(19, 6), make_pair(19, 7), make_pair(19, 8), make_pair(19, 9), make_pair(19, 10), make_pair(19, 11), make_pair(19, 12), make_pair(19, 13), make_pair(19, 14), make_pair(19, 15), make_pair(19, 16), make_pair(19, 17), make_pair(19, 18), make_pair(19, 19), make_pair(19, 20), make_pair(19, 21), make_pair(19, 22), make_pair(19, 23), make_pair(19, 24), 
make_pair(20, 4), make_pair(20, 5), make_pair(20, 6), make_pair(20, 7), make_pair(20, 8), make_pair(20, 9), make_pair(20, 10), make_pair(20, 11), make_pair(20, 12), make_pair(20, 13), make_pair(20, 14), make_pair(20, 15), make_pair(20, 16), make_pair(20, 17), make_pair(20, 18), make_pair(20, 19), make_pair(20, 20), make_pair(20, 21), make_pair(20, 22), make_pair(20, 23), make_pair(20, 24), 
make_pair(21, 4), make_pair(21, 5), make_pair(21, 6), make_pair(21, 7), make_pair(21, 8), make_pair(21, 9), make_pair(21, 10), make_pair(21, 11), make_pair(21, 12), make_pair(21, 13), make_pair(21, 14), make_pair(21, 15), make_pair(21, 16), make_pair(21, 17), make_pair(21, 18), make_pair(21, 19), make_pair(21, 20), make_pair(21, 21), make_pair(21, 22), make_pair(21, 23), make_pair(21, 24)
};
    chamber3 = {make_pair(10, 38), make_pair(10, 39), make_pair(10, 40), make_pair(10, 41), make_pair(10, 42), make_pair(10, 43), make_pair(10, 44), make_pair(10, 45), make_pair(10, 46), make_pair(10, 47), make_pair(10, 48), make_pair(10, 49), 
make_pair(11, 38), make_pair(11, 39), make_pair(11, 40), make_pair(11, 41), make_pair(11, 42), make_pair(11, 43), make_pair(11, 44), make_pair(11, 45), make_pair(11, 46), make_pair(11, 47), make_pair(11, 48), make_pair(11, 49), 
make_pair(12, 38), make_pair(12, 39), make_pair(12, 40), make_pair(12, 41), make_pair(12, 42), make_pair(12, 43), make_pair(12, 44), make_pair(12, 45), make_pair(12, 46), make_pair(12, 47), make_pair(12, 48), make_pair(12, 49)
};
    chamber4 = {make_pair(3, 39), make_pair(3, 40), make_pair(3, 41), make_pair(3, 42), make_pair(3, 43), make_pair(3, 44), make_pair(3, 45), make_pair(3, 46), make_pair(3, 47), make_pair(3, 48), make_pair(3, 49), make_pair(3, 50), make_pair(3, 51), make_pair(3, 52), make_pair(3, 53), make_pair(3, 54), make_pair(3, 55), make_pair(3, 56), make_pair(3, 57), make_pair(3, 58), make_pair(3, 59), make_pair(3, 60), make_pair(3, 61), 
make_pair(4, 39), make_pair(4, 40), make_pair(4, 41), make_pair(4, 42), make_pair(4, 43), make_pair(4, 44), make_pair(4, 45), make_pair(4, 46), make_pair(4, 47), make_pair(4, 48), make_pair(4, 49), make_pair(4, 50), make_pair(4, 51), make_pair(4, 52), make_pair(4, 53), make_pair(4, 54), make_pair(4, 55), make_pair(4, 56), make_pair(4, 57), make_pair(4, 58), make_pair(4, 59), make_pair(4, 60), make_pair(4, 61), 
make_pair(5, 39), make_pair(5, 40), make_pair(5, 41), make_pair(5, 42), make_pair(5, 43), make_pair(5, 44), make_pair(5, 45), make_pair(5, 46), make_pair(5, 47), make_pair(5, 48), make_pair(5, 49), make_pair(5, 50), make_pair(5, 51), make_pair(5, 52), make_pair(5, 53), make_pair(5, 54), make_pair(5, 55), make_pair(5, 56), make_pair(5, 57), make_pair(5, 58), make_pair(5, 59), make_pair(5, 60), make_pair(5, 61), make_pair(5, 62), make_pair(5, 63), make_pair(5, 64), make_pair(5, 65), make_pair(5, 66), make_pair(5, 67), make_pair(5, 68), make_pair(5, 69), 
make_pair(6, 39), make_pair(6, 40), make_pair(6, 41), make_pair(6, 42), make_pair(6, 43), make_pair(6, 44), make_pair(6, 45), make_pair(6, 46), make_pair(6, 47), make_pair(6, 48), make_pair(6, 49), make_pair(6, 50), make_pair(6, 51), make_pair(6, 52), make_pair(6, 53), make_pair(6, 54), make_pair(6, 55), make_pair(6, 56), make_pair(6, 57), make_pair(6, 58), make_pair(6, 59), make_pair(6, 60), make_pair(6, 61), make_pair(6, 62), make_pair(6, 63), make_pair(6, 64), make_pair(6, 65), make_pair(6, 66), make_pair(6, 67), make_pair(6, 68), make_pair(6, 69), make_pair(6, 70), make_pair(6, 71), make_pair(6, 72), 
make_pair(7, 61), make_pair(7, 62), make_pair(7, 63), make_pair(7, 64), make_pair(7, 65), make_pair(7, 66), make_pair(7, 67), make_pair(7, 68), make_pair(7, 69), make_pair(7, 70), make_pair(7, 71), make_pair(7, 72), make_pair(7, 73), make_pair(7, 74), make_pair(7, 75), make_pair(8, 61), make_pair(8, 62), make_pair(8, 63), make_pair(8, 64), make_pair(8, 65), make_pair(8, 66), make_pair(8, 67), make_pair(8, 68), make_pair(8, 69), make_pair(8, 70), make_pair(8, 71), make_pair(8, 72), make_pair(8, 73), make_pair(8, 74), make_pair(8, 75), 
make_pair(9, 61), make_pair(9, 62), make_pair(9, 63), make_pair(9, 64), make_pair(9, 65), make_pair(9, 66), make_pair(9, 67), make_pair(9, 68), make_pair(9, 69), make_pair(9, 70), make_pair(9, 71), make_pair(9, 72), make_pair(9, 73), make_pair(9, 74), make_pair(9, 75), 
make_pair(10, 61), make_pair(10, 62), make_pair(10, 63), make_pair(10, 64), make_pair(10, 65), make_pair(10, 66), make_pair(10, 67), make_pair(10, 68), make_pair(10, 69), make_pair(10, 70), make_pair(10, 71), make_pair(10, 72), make_pair(10, 73), make_pair(10, 74), make_pair(10, 75), 
make_pair(11, 61), make_pair(11, 62), make_pair(11, 63), make_pair(11, 64), make_pair(11, 65), make_pair(11, 66), make_pair(11, 67), make_pair(11, 68), make_pair(11, 69), make_pair(11, 70), make_pair(11, 71), make_pair(11, 72), make_pair(11, 73), make_pair(11, 74), make_pair(11, 75), 
make_pair(12, 61), make_pair(12, 62), make_pair(12, 63), make_pair(12, 64), make_pair(12, 65), make_pair(12, 66), make_pair(12, 67), make_pair(12, 68), make_pair(12, 69), make_pair(12, 70), make_pair(12, 71), make_pair(12, 72), make_pair(12, 73), make_pair(12, 74), make_pair(12, 75)
};
    chamber5 = {make_pair(16, 65), make_pair(16, 66), make_pair(16, 67), make_pair(16, 68), make_pair(16, 69), make_pair(16, 70), make_pair(16, 71), make_pair(16, 72), make_pair(16, 73), make_pair(16, 74), make_pair(16, 75), 
make_pair(17, 65), make_pair(17, 66), make_pair(17, 67), make_pair(17, 68), make_pair(17, 69), make_pair(17, 70), make_pair(17, 71), make_pair(17, 72), make_pair(17, 73), make_pair(17, 74), make_pair(17, 75), 
make_pair(18, 65), make_pair(18, 66), make_pair(18, 67), make_pair(18, 68), make_pair(18, 69), make_pair(18, 70), make_pair(18, 71), make_pair(18, 72), make_pair(18, 73), make_pair(18, 74), make_pair(18, 75), 
make_pair(19, 37), make_pair(19, 38), make_pair(19, 39), make_pair(19, 40), make_pair(19, 41), make_pair(19, 42), make_pair(19, 43), make_pair(19, 44), make_pair(19, 45), make_pair(19, 46), make_pair(19, 47), make_pair(19, 48), make_pair(19, 49), make_pair(19, 50), make_pair(19, 51), make_pair(19, 52), make_pair(19, 53), make_pair(19, 54), make_pair(19, 55), make_pair(19, 56), make_pair(19, 57), make_pair(19, 58), make_pair(19, 59), make_pair(19, 60), make_pair(19, 61), make_pair(19, 62), make_pair(19, 63), make_pair(19, 64), make_pair(19, 65), make_pair(19, 66), make_pair(19, 67), make_pair(19, 68), make_pair(19, 69), make_pair(19, 70), make_pair(19, 71), make_pair(19, 72), make_pair(19, 73), make_pair(19, 74), make_pair(19, 75), 
make_pair(20, 37), make_pair(20, 38), make_pair(20, 39), make_pair(20, 40), make_pair(20, 41), make_pair(20, 42), make_pair(20, 43), make_pair(20, 44), make_pair(20, 45), make_pair(20, 46), make_pair(20, 47), make_pair(20, 48), make_pair(20, 49), make_pair(20, 50), make_pair(20, 51), make_pair(20, 52), make_pair(20, 53), make_pair(20, 54), make_pair(20, 55), make_pair(20, 56), make_pair(20, 57), make_pair(20, 58), make_pair(20, 59), make_pair(20, 60), make_pair(20, 61), make_pair(20, 62), make_pair(20, 63), make_pair(20, 64), make_pair(20, 65), make_pair(20, 66), make_pair(20, 67), make_pair(20, 68), make_pair(20, 69), make_pair(20, 70), make_pair(20, 71), make_pair(20, 72), make_pair(20, 73), make_pair(20, 74), make_pair(20, 75), 
make_pair(21, 37), make_pair(21, 38), make_pair(21, 39), make_pair(21, 40), make_pair(21, 41), make_pair(21, 42), make_pair(21, 43), make_pair(21, 44), make_pair(21, 45), make_pair(21, 46), make_pair(21, 47), make_pair(21, 48), make_pair(21, 49), make_pair(21, 50), make_pair(21, 51), make_pair(21, 52), make_pair(21, 53), make_pair(21, 54), make_pair(21, 55), make_pair(21, 56), make_pair(21, 57), make_pair(21, 58), make_pair(21, 59), make_pair(21, 60), make_pair(21, 61), make_pair(21, 62), make_pair(21, 63), make_pair(21, 64), make_pair(21, 65), make_pair(21, 66), make_pair(21, 67), make_pair(21, 68), make_pair(21, 69), make_pair(21, 70), make_pair(21, 71), make_pair(21, 72), make_pair(21, 73), make_pair(21, 74), make_pair(21, 75)
};
    
    vector<vector<pair<int,int>>> chambers = {chamber1, chamber2, chamber3,
                                              chamber4, chamber5};
    
    
    // place character randomly
    // gets a chamber and also gets a point in that chamber to put the character on
    int playerchamber = rand() % 5;
    int playerpoint = rand() % chambers[playerchamber].size();
    
    int i_char = chambers[playerchamber][playerpoint].first;
    int j_char = chambers[playerchamber][playerpoint].second;
    
    board[i_char][j_char] = hero[0]; // puts the hero on a space in the chamber
    
    int stairchamber = rand() % 5; // get the chamber
    while (stairchamber == playerchamber){ // if the player and stair are in the same chamber,
        stairchamber = rand() % 5; // keep going through to find a different chamber than the hero is in
    }
    //cout << "player chamber " << playerchamber << endl;
    //cout << "stairchamber " << stairchamber << endl;
    
    int stairpoint = rand() % chambers[stairchamber].size(); // get a random point in the chamber
    
    int i_stair = chambers[stairchamber][stairpoint].first;
    int j_stair = chambers[stairchamber][stairpoint].second;
    
    board[i_stair][j_stair] = '\\';
    
    int potion_count = 10;
    
    const char tile = '.';
    
    for (int i = 0; i < potion_count; i++) {
        int pottype = rand() % 6;
        int potchamber = rand() % 5;
        int potpoint = rand() % chambers[potchamber].size();
        bool notPlaced = true;
        while (notPlaced){ // we need this if the board has an object on it already
            int i_pot = chambers[potchamber][potpoint].first;
            int j_pot = chambers[potchamber][potpoint].second;
            if (board[i_pot][j_pot] == tile){
                notPlaced = false;
                board[i_pot][j_pot] = potionNum[pottype];
            } else{
                potpoint = rand() % chambers[potchamber].size();
             }
        }
    }
    
    int gold_count = 10;
    const string treasure = "679";
    
    for (int i = 0; i < gold_count; i++) {
        int goldtype = rand() % 8;
        if (goldtype <= 4){ // normal hoard
            goldtype = 0;
        } else if (goldtype == 5 || goldtype == 6){ // small hoard
            goldtype = 1;
        } else if (goldtype == 7){ // dragon hoard
            goldtype = 2; // we are using the const treasure above
        }
        int goldchamber = rand() % 5;
        int goldpoint = rand() % chambers[goldchamber].size();
        bool notPlaced = true;
        while (notPlaced){ // we need this if the board has an object on it already
            int i_gold = chambers[goldchamber][goldpoint].first;
            int j_gold = chambers[goldchamber][goldpoint].second;
            if (board[i_gold][j_gold] == tile){
                notPlaced = false;
                board[i_gold][j_gold] = treasure[goldtype];
            } else{
                goldpoint = rand() % chambers[goldchamber].size();
             }
        }
        int i_gold = chambers[goldchamber][goldpoint].first;
        int j_gold = chambers[goldchamber][goldpoint].second;
        
        vector<string> dir = {"no", "so", "ea", "we", "ne", "nw", "se", "sw"};
        int tolerance = 0; // we need the tolernace in case the dragon can't be placed
        // this is because there may be 8 potions placed around the gold
        // although this is unlikley, we like to consider all the cases.
        if (board[i_gold][j_gold] == dragonhoard){
            bool moved = false;
            while(!moved && tolerance != 100){
                int randomDir = rand() % 8; 
                vector<int> dragonPos = changeDir(i_gold, j_gold, dir[randomDir]);
                if (board[dragonPos[0]][dragonPos[1]] == tile){
                    board[dragonPos[0]][dragonPos[1]] = dragon;
                    moved = true;
                }
                tolerance++;
            }
        }
        if (tolerance == 100){ // the dragon couldn't be placed
            board[i_gold][j_gold] = tile; // reset the gold to a tile
            i = i - 1; // go back one step 
            continue; // try again
        }
    }
    
    int enemy_count = 20;
    
    const string enemy = "HWLEOM";
    
    for (int i = 0; i < enemy_count; i++) {
        int enemytype = rand() % 18;
        if (enemytype <= 3){ // human
            enemytype = 0;
        } else if ((enemytype >= 4) && (enemytype <= 6)){ // dwarf
            enemytype = 1;
        } else if ((enemytype >= 7) && (enemytype <= 11)){ // halfling
            enemytype = 2; 
        } else if ((enemytype >= 12) && (enemytype <= 13)){ // elf
            enemytype = 3;
        } else if ((enemytype >= 14) && (enemytype <= 15)){ // orc
            enemytype = 4;
        } else if ((enemytype >= 16) && (enemytype <= 17)){
            enemytype = 5;
        }
        int enemychamber = rand() % 5;
        int enemypoint = rand() % chambers[enemychamber].size();
        bool notPlaced = true;
        while (notPlaced){ // we need this if the board has an object on it already
            int i_enemy = chambers[enemychamber][enemypoint].first;
            int j_enemy = chambers[enemychamber][enemypoint].second;
            if (board[i_enemy][j_enemy] == tile){
                notPlaced = false;
                board[i_enemy][j_enemy] = enemy[enemytype];
            } else{
                enemypoint = rand() % chambers[enemychamber].size();
             }
        }
    }
    
    return board;
    
}    

string Board::computeNextIteration(shared_ptr<Character> c){
    vector<string> dir = {"no", "so", "ea", "we", "ne", "nw", "se", "sw"};
    vector<pair<int, int>> alreadyMoved; // we need to keep track to see if we already moved the enemy
    //cout << "start iteration " << endl;
    string action;
    
    // check for the enemies and move them as appropirate
    for (int i = 1; i < this->height-1; i++) {
        for (int j = 1; j < this->width-1; j++) {
            // check to see if we moved the enemy already
            pair<int,int> temp = make_pair(i, j);
            bool moved = false;
            for (int k = 0; k < alreadyMoved.size(); k++) {
                if (temp == alreadyMoved[k]){
                    moved = true;
                    break;
                }
            }
            // add this in only if the enemy should stop
            bool cont = true;
            
            for (int a = i-1; a <= i+1; a++) {
                for (int b = j-1; b <= j+1; b++) {
                    if (board[a][b].o.get() != nullptr &&
                        hero.find(board[a][b].o.get()->getDisplay()) != -1){
                        cont = false;
                        break;
                    }
                }
            }
            
            if (!cont){
                continue;
            }

            if (board[i][j].o.get() != nullptr &&
                enemy.find(board[i][j].o.get()->getDisplay()) != -1 &&
                board[i][j].o.get()->getDisplay() != dragon && !moved &&
                board[i][j].getInfo().t == CellType::TILE){ 
                int tolerance = 0;
                // try moving it 
                while (!moved && tolerance < 100){
                    int num = rand() % dir.size();
                    vector<int> randomDir = changeDir(i, j, dir[num]);
                    if (board[randomDir[0]][randomDir[1]].o.get() == nullptr &&
                        board[randomDir[0]][randomDir[1]].getInfo().t == CellType::TILE){
                        //cout << "old x: " << i << " y: " << j;
                        moved = true;
                        // update position of the enemy
                        board[i][j].o.get()->setX(randomDir[0]);
                        board[i][j].o.get()->setY(randomDir[1]);
                        alreadyMoved.push_back(make_pair(randomDir[0], randomDir[1]));
                        //cout << " to x: " << randomDir[0] << " y: " << randomDir[1] << endl;
                        //cout << "display: " << board[i][j].o.get()->getDisplay() << endl;
                        board[i][j].o.swap(board[randomDir[0]][randomDir[1]].o);
                        // notify the display
                        board[i][j].notifyObservers(CellType::TILE);
                        if (board[i][j].o.get() != nullptr){
                            action += "this shouldn't appear";
                        }
                        board[randomDir[0]][randomDir[1]].notifyObservers(CellType::TILE);
                        break;
                    }
                    // if the loop has to go through again at least 100 times
                    // then the enemy won't be moved. this is for when
                    // the space the enemy wants to move to is occupied
                    tolerance++;
                }
            }
        }
    }
    
    // get the position of the character so we can attack accordingly
    int i = c.get()->getX();
    int j = c.get()->getY();
    bool dragonattack = false;
    // now check for enemies around the hero
    // and attack the hero accordingly
    
    //cout << "i: " << i << " j: " << j << endl;
    for (int a = i-1; a <= i+1; a++) {
        for (int b = j-1; b <= j+1; b++) {
            if (board[a][b].o.get() != nullptr &&
                enemy.find(board[a][b].o.get()->getDisplay()) != -1){
                shared_ptr<Enemy> e = dynamic_pointer_cast<Enemy>(board[a][b].o);
                int before_hp = c.get()->gethp();
                bool attacked = false;
                if (e.get() != nullptr && e.get()->getDisplay() == dragon){
                    e.get()->makeHostile(); // make dragon hostile
                    e.get()->attack(c); // dragon attacks the hero
                    dragonattack = true;
                    attacked = true;
                } else if (e.get() != nullptr && e.get()->isHostile()){
                    e.get()->attack(c); // enemy attacks hero
                    attacked = true;
                } else if (e.get() != nullptr && e.get()->getDisplay() == merchant){
                    // the merchant doesn't attack
                }else{
                    action += "something went wrong using attacking hero";
                }
                int after_hp = before_hp - c.get()->gethp();
                char enemy = e.get()->getDisplay();
                if (attacked){
                    action = action + enemy + " deals " +
                        to_str(after_hp) + " damage to PC. ";
                }
            } else if (board[a][b].o.get() != nullptr &&
                       board[a][b].o.get()->getDisplay() == gold &&
                       !dragonattack){ // it's a gold
                // first find if it's a dragon hoard
                shared_ptr<Treasure> t = dynamic_pointer_cast<Treasure>(board[a][b].o);
                if (t.get()->getType() != dragonhoard){
                    t.reset();
                    break;
                }
                // find the dragon and make it hostile, and then make the dragon attack
                // when the character touches a hoard
                int k = t.get()->getX();
                int l = t.get()->getY();
                //cout << "char: x: " << c.get()->getX() << " y: " << c.get()->getY() << endl;
                for (int x = k-1; x <= k+1; x++) {
                    for (int y = l-1; y <= l+1; y++) {
                       //cout << "checking: x: " << x << " y: " << y << endl;
                        if (board[x][y].o.get() != nullptr &&
                           board[x][y].o.get()->getDisplay() == dragon){ // if it's a dragon
                            shared_ptr<Enemy> e = dynamic_pointer_cast<Enemy>(board[x][y].o);
                            int before_hp = c.get()->gethp();
                            e.get()->makeHostile();
                            e.get()->attack(c);
                            int after_hp = before_hp - c.get()->gethp();
                            char enemy = e.get()->getDisplay();
                            action = action + enemy + " deals " + 
                            to_str(after_hp) + " damage to PC. ";
                        }
                    }
                }
            }
        }
    }
    return action.substr(0, action.size()-1); // chop off the space at the end
}

ostream &operator<<(ostream &out, const Board &b){
    out << *(b.td.get());
    return out;
}


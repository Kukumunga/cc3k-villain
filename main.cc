/* 
 * File:   main.cc
 * Author: Nathan Duma
 *
 * Created on March 26, 2017, 7:23 AM
 */

#include <string>
#include <iostream>
#include <vector>

#include "cell.h"
#include "gameobject.h"
#include "board.h"
#include <algorithm>
#include <fstream>
#include <string>
#include "hero.h"
#include <ctime>
#include <map>
#include <sstream>
#include <memory>
#include "cheat.h"

using namespace std;


static bool validDir(string command){
return (command == "no" || command == "so" || command == "ea"
                || command == "we" || command == "nw" || command == "se"
                || command == "sw" || command == "ne");
}

static string to_str(int num){
    ostringstream s;
    s << num;
    return s.str();
}
static void score(double g, char r){
    double finalgold = g;
    if(r == 's'){
        finalgold*=1.5;
    }
    cout << "Your final score is " << finalgold << endl;
}
static void lose(){
    
    cout << "You lose" << endl;
    

    // display Feridun Hamdullaphur here
}
static void characterselect(){
    cout << "Choose your Hero!" << endl;
    cout << "s: Shade   d: Drow    v: Vampire" << endl;
    cout << "g: Goblin  t: Troll r: Rubber Ducky Debugger 1337" << endl;
    cout << "Please enter the race of the hero: ";

}
static void ascii(string file){
    ifstream winning(file);
    cout << winning.rdbuf();
    cout << endl;
}

int main(int argc, char* argv[]) {
    srand((unsigned)time(0)); // produce random number
    
    // the name of the file that contains the board
    string filename;
    int makerandom = false;
    if (argc == 2){ // a filename has been provided by the command line
        filename = argv[1];
    } else{ // otherwise the default empty  will be used, then generated upon
        filename = "empty.txt";
        makerandom = true;
    }
    
    ifstream input(filename);
    
    
    // filenames for the winning and losing files
    string feridunFile = "feridun.txt";
    string gameoverFile = "gameover.txt";
    string winFile = "win.txt";
    string gooseFile = "goose.txt";
    string duckFile = "duck.txt";
    
    // read in the 5 boards
    int possibleBoards = 5;
    int height = 25;
    int width = 79;
    string s;
    vector<vector<vector<char>>> board;
    
    for (int j = 0; j < possibleBoards; j++) {
        vector<vector<char>> floor(height);
        for (int i = 0; i < height; i++) {
            // push the vector of characters onto the vector
            floor.push_back(vector<char>());
            floor[i].resize(width);
            getline(input, s);
            for (int k = 0; k < width; k++) {
                floor[i][k] = s[k];
            }
        }
        board.push_back(floor);
    }

    input.close();
    int currfloor = 0;
    int winningFloor = 5;
    char race;
    string result;
    bool makehostile = false;
    // keeps track of the last valid command
    vector<string> actions;
    string spawnaction = "Player has spawned";
    actions.push_back(spawnaction);
    //mapping created for board display
    map<char,string> character;
    character['s'] = "Shade";
    character['d'] = "Drow";
    character['v'] = "Vampire";
    character['g'] = "Goblin";
    character['t'] = "Troll";
    character['r'] = "Rubber Ducky Debugger 1337";
    character['z'] = "Debug";

    // get the race and make a new hero
    bool found = false;
    //characterselect();

    while(!found){
        characterselect();
        cin >> result;
        if (result.length() == 1){
            race = result[0];
            if(character.find(race) == character.end()){
            cout << "Invalid character selection!" << endl;
            //characterselect();
            }
            else {
                found = true;
            } 
        }
        else {
            cout << "Invalid character selection!" << endl;
        }
    } 
    shared_ptr<Character> h(new Hero(race));
    unique_ptr<Board> b(new Board(board[currfloor], h, currfloor+1, makerandom, makehostile));
    bool freeze = false;
    bool restart = false;
    // command interpreter
    currfloor = 1;
    while (1){
        string command;
        // read in the next floor once they are off this one
        if (b.get()->getFloor() == winningFloor){ // win
            ascii(gooseFile);
            ascii(winFile);
            score(h->getGold(),h->getRace());
            bool end = false;
            // display Mr. Goose Here
            // also need to add option to restart game
            
            currfloor = 0;
            found = false;
                    
            makehostile = false;
            while (!restart){
                cout << "Would you like to play again? [y/n]" << endl;
                cin >> command;
                
                if(command == "n"){
                    
                    end = true;
                    break;
                }
                else if(command == "y"){
                    // clear everything
                    // add helper for restart;
                    
                    restart = true;
                    // reset the character
                    while(!found){
                        characterselect();
                        cin >> result;
                        if (result.length() == 1){
                            race = result[0];
                            if(character.find(race) == character.end()){
                            cout << "Invalid character selection!" << endl;
                            //characterselect();
                            }
                            else {
                                found = true;
                            } 
                        }
                        else {
                            cout << "Invalid character selection!" << endl;
                        }
                    } 
                    
                    h.reset(new Hero(race));
                    b.reset(new Board(board[currfloor], h, currfloor+1, makerandom, makehostile));
                    currfloor++;
                    continue;  
                }
                
                else{
                    cout << "Invalid command" << endl;
                    //cout << "Please enter y for yes or n for no" << endl;
                }
                
            }
            if (end == true){break;}
           
        } else if (h->gethp() == 0){ // lose
            ascii(feridunFile);
            ascii(gameoverFile);
            score(h->getGold(),h->getRace());
            bool end = false;
            // display Mr. Goose Here
            // also need to add option to restart game
            
            currfloor = 0;
            found = false;
                    
            makehostile = false;
            while (!restart){
                cout << "Would you like to play again? [y/n]" << endl;
                cin >> command;
                
                if(command == "n"){
                    
                    end = true;
                    break;
                }
                else if(command == "y"){
                    // clear everything
                    // add helper for restart;
                    
                    restart = true;
                    // reset the character
                    while(!found){
                        characterselect();
                        cin >> result;
                        if (result.length() == 1){
                            race = result[0];
                            if(character.find(race) == character.end()){
                            cout << "Invalid character selection!" << endl;
                            //characterselect();
                            }
                            else {
                                found = true;
                            } 
                        }
                        else {
                            cout << "Invalid character selection!" << endl;
                        }
                    } 
                    
                    h.reset(new Hero(race));
                    b.reset(new Board(board[currfloor], h, currfloor+1, makerandom, makehostile));
                    currfloor++;
                    continue;  
                }
                
                else{
                    cout << "Invalid command" << endl;
                    //cout << "Please enter y for yes or n for no" << endl;
                }
                
            }
            if (end == true){break;}
            //break;
        } 
        else if (currfloor < b.get()->getFloor()){ // go to next floor
            b.reset(new Board(board[currfloor], h, currfloor+1, makerandom, makehostile));
            currfloor++;
        }
        int g = h->getGold(); //player's currrent gold
        cout << *b.get() << endl; // print the board every iteration
        cout << "Race: " << character[h->getRace()] << " Gold: " << g;
        int goldlength = to_str(g).length() + 7;
        int floordisplay = 6 + character[h->getRace()].length() + goldlength;
        for (int i = floordisplay; i < 68; i++){
            cout << " ";
        }
        cout << "Floor " << b.get()->getFloor() << endl;
        cout << "HP: " << h->gethp() << endl;
        cout << "Atk: " << h->getatk() << endl;
        cout << "Def: " << h->getdef() << endl;
        if(restart == true){
        	actions.clear();
	        actions.push_back(spawnaction);
	        restart = false;
        }
       
        if(!actions.empty()){
            cout << "Actions: ";
            for (int i = 0; i < actions.size(); i++) {
                cout << actions[i] << " ";
            }
            cout << endl;
            actions.clear();
        }
        else {
            cout << "Actions: " << endl;
        }
        cout << "What is your next move? ";
        if (!(cin >> command)){
            break;
        };
        if (h.get()->getRace() == 'd'){
            ascii(duckFile);
        }
        if (command == "q"){
            lose();
            break;
        } else if (validDir(command)){
            actions.push_back(b.get()->move(h, command));
            if (actions[0] == "Invalid move!"){
                continue;
            }
        }
        else if (command == "u"){ // use potion
            
            cin >> command; // get the direction
            
            if (validDir(command)){
               actions.push_back(b.get()->usePotion(h, command));
               if (actions.back() == "PC tried to use a potion that was not there!"){
                   continue;
               }
            } else{
                actions.push_back("Invalid direction!");
                continue;
            }
        } else if (command == "a"){ // attack
            cin >> command; // get the direction
            
            if (validDir(command)){
                actions.push_back(b.get()->attack(h, command));
                if (actions.back() == "PC tried to attack an enemy that's not there!"){
                    continue;
                }
            } else{
                actions.push_back("Invalid direction");
                continue;
            }
            
        }
        else if (command == "nomair"){
            string mycode;
            Cheat c;
            cin >> mycode;
            actions.push_back(c.imacheater(h.get(),mycode));
            continue;

        }
        else if (command == "f"){
            freeze = !freeze;
            // stop enemies from moving
            if (!freeze){
                actions.push_back("Enemies are now un-frozen");
            } else{
                actions.push_back("enemies are now frozen");
            }
            continue;
        } else if (command == "r"){
            // clear everything
            currfloor = 0;
            // reset the character
            found = false;
            while(!found){
                characterselect();
                cin >> result;
                if (result.length() == 1){
                    race = result[0];
                    if(character.find(race) == character.end()){
                    cout << "Invalid character selection!" << endl;
                    //characterselect();
                    }
                    else {
                        found = true;
                    } 
                }
                else {
                    cout << "Invalid character selection!" << endl;
                }
            } 
            restart = true;
            makehostile = false; 
            h.reset(new Hero(race));
            //b.get()->clearBoard(board[currfloor], h, currfloor+1, makehostile);
            continue;
        } else{
            actions.push_back("Invalid command");
            continue;
        }
        // compute next iteration. move the enemies around and stuff
        if (!freeze){ // if the f command has not been initiated
            actions.push_back(b.get()->computeNextIteration(h));
        }
        
        if (!makehostile){
            makehostile = b.get()->getHostile();
        }
    }
}


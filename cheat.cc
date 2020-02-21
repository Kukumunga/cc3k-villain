#include <string>
#include "character.h"
#include "hero.h"
#include "cheat.h"

Cheat::Cheat(){
	cout << "Please enter a cheat code" << endl;
}
Cheat::~Cheat(){}
string Cheat::to_str(int num){
    ostringstream s;
    s << num;
    return s.str();
}
string Cheat::imacheater(Character *c,string code){
	string response;
	if(code == this->cheat1){
		int curr = c->gethp();
		int newhp = curr + 100;
		c->sethp(newhp);
		int newcurr = c->gethp();
		int diff = newcurr - curr;
		response = "You health has been restored by " + to_str(diff) + "... cheater";
		return response;
	}
	else if (code == this->cheat2){
		int curr = c->getatk();
		curr+=25;
		c->setatk(curr);
		response = "You gained 25 atk... cheater";
		return response;
	}
	else if (code == this->cheat3){
		int curr = c->getdef();
		curr+=25;
		c->setdef(curr);
		response = "You gained 25 def... cheater";
		return response;
	}
	else{
		response = "Invalid cheat code";
		return response;
	}
}
#include <future>

#include "potion.h"
#include "hero.h"

Potion::Potion(char type){
    this->display = 'P';
    this->type = type;
    //cout << "postion type: " << this->type << endl;
}

void Potion::useItem(shared_ptr<Hero> h){ //depends on how we create a hero
    
	char race = h.get()->getRace();
        double value;
	double multiplier = 1.0;
	if (race == 'd'){
		multiplier = 1.5;
	}
	if(this->type == '0'){
		value = h.get()->gethp() + 10.0 * multiplier ;
		h.get()->sethp((int)value);
        }
	else if(this->type == '1'){
		value = h.get()->getatk() + 5.0 * multiplier;
		h.get()->setatk((int)value);
        }
	else if(this->type == '2'){
	        value = h.get()->getdef() + 5.0 * multiplier;
		h.get()->setdef((int)value);
        }
	else if(this->type == '3'){
	    value = h.get()->gethp() - 10.0 * multiplier;
            h.get()->sethp((int)value);

        }
	else if(this->type == '4'){
		value = h.get()->getatk() - 5.0 * multiplier;
		h.get()->setatk((int)value);
        }
	else if(this->type == '5'){
		value = h.get()->getdef() - 5.0 * multiplier;
		h.get()->setdef((int)value);

	}
        //cout << "value is " << value << endl;
        //cout << "potion type is " << this->type << endl;
}

char Potion::getDisplay(){
    return this->display;
}

char Potion::getType(){
    return this->type;
}


Potion::~Potion() {
}


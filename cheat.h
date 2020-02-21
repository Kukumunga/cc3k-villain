#ifndef CHEATS_H
#define CHEATS_H

#include <string>
#include "character.h"
#include "hero.h"

using namespace std;

class Cheat {
	const string cheat1 = "246";
	const string cheat2 = "42";
	const string cheat3 = "1171";
	string to_str(int num);
public:
	Cheat();
	string imacheater(Character *c,string code);
	~Cheat();
};

#endif

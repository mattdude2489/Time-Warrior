#pragma once
/*
This class handles XP Gain and leveling for
Characters (15), Melee Attacks (40), Magic Attacks (40)
*/

/*XP Gain Formula | f(n){n > 0} = f(n-1) + n
f(1) = 0 + 1 = 1
f(2) = 1 + 2 = 3
f(3) = 3 + 3 = 6
*/

//contains the level and XP required to reach it
struct LEVEL_SYSTEM
{
	int level;
	int xpRequired;
};

class BaseLeveler
{
public:
	/*--- Constructor ---*/
	BaseLeveler(void);
	~BaseLeveler(void);
	/*--- Base Functions ---*/
	/*--- XP Gain Functions ---*/
	/*--- Level checking fuctions ---*/
	//check the current level and XP if, XP
	/*--- Accessor/Mutators ---*/
private:
	/*--- Main Functions ---*/
	/*--- Outside Variables ---*/
	/*--- Class Variables ---*/
	// This goes up to L40
	LEVEL_SYSTEM *XP_KEY;
};
#ifndef _GAMEOWNEDSTATES_H_
#define _GAMEOWNEDSTATES_H_

#include "State.h"

class Game_State;

class Load_State : public State<Game_State> {
public:
	
	/** default constructor */
	Load_State();
	/** deconstructor */
	~Load_State();

	/** instance - returns singleton instance of menu state */
	static Load_State * instance();
	/** enter - executes when the menu state is entered */
	virtual void enter(Game_State * a_pvEngine);
	/** execute - executes during normal operation of menu state */
	virtual void execute(Game_State * a_pvEngine);
	/** exit - executes when the menu state is left */
	virtual void exit(Game_State * a_pvEngine);
};

#endif

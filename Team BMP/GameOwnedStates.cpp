#include "GameOwnedStates.h"

/** default constructor */
Load_State::Load_State() {
}

/** deconstructor */
Load_State::~Load_State() {
}



/** instance - returns singleton instance of menu state */
Load_State * Load_State::instance() {
	static Load_State instance;
	return &instance;
}

/** enter - executes when the menu state is entered */
void Load_State::enter(Game_State * a_pvEngine) {
}

/** execute - executes during normal operation of menu state */
void Load_State::execute(Game_State * a_pvEngine) {
}

/** exit - executes when the menu state is left */
void Load_State::exit(Game_State * a_pvEngine) {
}

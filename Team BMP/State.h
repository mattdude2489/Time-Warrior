#ifndef _STATE_H_
#define _STATE_H_

#include "Global.h"
#include "sdl\SDL.h"

/*
State Container
Author: Jared Bills
*/
template<class CUSTOM_CLASS>
class State {

protected:

	/** pointer to the next state to execute */
	State<CUSTOM_CLASS> * m_pvNextState;
	/** pointer to the previous state executed prior to this one */
	State<CUSTOM_CLASS> * m_pvPrevState;

public:
	
	/** Constructor */
	State() : m_pvNextState(NULL), m_pvPrevState(NULL) {}
	/** Destructor */
	~State() {}

	/** setNextState - sets the next state to the specified */
	void setNextState(State<CUSTOM_CLASS> * a_pvNextState) { m_pvNextState = a_pvNextState; }
	/** setPrevState - sets the previous state to the specified */
	void setPrevState(State<CUSTOM_CLASS> * a_pvPrevState) { m_pvPrevState = a_pvPrevState; }
	/** getNextState - gets the next state */
	State<CUSTOM_CLASS> * getNextState() const { return m_pvNextState; }
	/** getPrevState - gets the previous state */
	State<CUSTOM_CLASS> * getPrevState() const { return m_pvPrevState; }

	/** enter - executes when a state is entered */
	virtual void enter(CUSTOM_CLASS *) = 0;
	/** execute - normal update function for the given state */
	virtual void execute(CUSTOM_CLASS *) = 0;
	/** exit - executes when a state is exited */
	virtual void exit(CUSTOM_CLASS *) = 0;

};

#endif

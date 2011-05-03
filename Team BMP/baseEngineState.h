#pragma once
#include "baseEngine.h"
//A very simple state struct.

class baseEngine;

class actualGameState : public State
{
public:
	void enter() {}
	void execute() {} //Input Logic Draw
	void exit() {}
	static actualGameState* instance() {static actualGameState instance; return &instance;}
};

class titleScreenState : public State
{
public:
	void enter() {}
	void execute() {}//Title screen stuffs.
	void exit() {baseEngine::cState = actualGameState::instance(); baseEngine::cState->enter(); }
	static titleScreenState* instance() {static titleScreenState instance; return &instance;}
};

class splashScreenState : public State
{
public:
	void enter() {}
	void execute() {} //Timer or something similar.
	void exit() {baseEngine::cState = titleScreenState::instance(); baseEngine::cState->enter(); }
	static splashScreenState* instance() {static splashScreenState instance; return &instance;}
};
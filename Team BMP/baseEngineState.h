#pragma once
#include "baseEngine.h"
#include "srect.h"
//A very simple state struct.

//SDL includes and defines
#define SDLDELAY		17
#include "sdl/sdl.h"
#include <stdio.h>
#include <conio.h>

//THIS NEXT LINE IS OMGWTFDANGEROUS, BE CAREFUL WHEN USING THIS LINE.
#define SDL_SCREEN_STARTUP			SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE)

class baseEngine;

//States:
/*
	actualGameState							-
		NetworkGameState				-
			ServerGameState					-
			ClientGameState					-
				-NewGameState			-
				-LoadGameState			-
	titleScreenState				-
		NewGameState					-
		LoadGameState					-
		NetworkGameState				-
	splashScreenState			-

	actualGameState will be the single-player. It won't worry about networking at all.
	titleScreenState will be the title screen. It has three states that it can go to.
		Each of those states are different ways to enter the actual game state.
		the Network game state has two other states, the server game and the client game. Both deal with networking.
	Splash screen state does...literally nothing.
*/

class actualGameState : public State
{
private:
	//INCLUDE SDL STUFF HERES
	SDL_Surface * screen;
	UserInput * stateUI;
	//Uint32 then = SDL_GetTicks(), now, passed, second = 0;
	Uint32 then, now, passed, second;
	TTtext fps;
	MyFont myfps;
	char cfps[10];
	int ifps;
public:
	void enter(baseEngine* be) 
	{
		screen = SDL_SCREEN_STARTUP;
		stateUI = NULL;
		then = SDL_GetTicks();
		now = passed = second = 0;
		myfps.setFont(FONTSIZE);
		fps.setFont(myfps.getFont());
		ifps = 0;
		fps.setMessage("0");
		be->getPlayer()->setGamePlayed(true);
	}
	void execute(baseEngine* be) 
	{
		ifps++;
		now = SDL_GetTicks();
		passed = now - then;
		then = now;
		if(now - second > 1000)
		{
			sprintf_s(cfps, "%i", ifps);
			fps.setMessage(cfps); //Set message? Could setMessage be causing a memory leak? I don't think so, but who knows?
			ifps = 0;
			second = now;
		}
		if(stateUI != NULL)
		{
			be->getPlayer()->handleInput(stateUI, be->getWorld(), be->getAH());
			be->getWorld()->update(passed);
			be->getAH()->update(be->getWorld()->getCurrentWorld());
			be->getHUD()->updateHud(be->getPlayer(), stateUI, passed);
		}

		SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));
		be->getWorld()->draw(screen);
		be->getHUD()->draw(screen);
		fps.printMessage(screen, 0, 0);
		SDL_Flip(screen);
		if(stateUI != NULL)
			if(stateUI->getX())
				exit(be);
		SDL_Delay(SDLDELAY);
	} //Input Logic Draw
	void exit(baseEngine* be) 
	{
		SDL_FreeSurface(screen);
		be->getPlayer()->destroyPlayer();
		be->getWorld()->destroyWorld();
		be->goToTitleScreen();
	}
	static actualGameState* instance() {static actualGameState instance; return &instance;}
	void handleInput(UserInput * obj) {stateUI = obj;}
};


class loadGameState: public State
{
private:
	UserInput * stateUI;	//The UserInput. We all know how this goes.
	SDL_Surface * screen;	//The SDL_Surface of which this State writes to.
	int optionOfReturn;		//The erroring integer, or the saveFile that the user has selected.
	int saveFiles;			//The number of Save Files located in the file.
	SRect * loadRects;		//...The soon to be array of rectangles that allow the user to switch which save file.
	TTtext playerMessages;	//Any messages that may need to occur, such as erroring.
	TTtext * loadMessages;	//I'm going to go to hell for this, aren't I?
	int checkClick;
public:
	void enter(baseEngine *be)
	{
		screen = SDL_SCREEN_STARTUP;
		optionOfReturn = 0; //Initialize the erroring integer.
		checkClick = -1;
		saveFiles = 0;
		FILE * hi;
		hi = fopen("playerSave.txt", "r"); //Open the file for reading.
		//if(hi == NULL)
		//{
		//	optionOfReturn = -1; //Go back to Title Screen if the FILE cannot be opened, for some odd weird reason.
		//	exit(be);
		//}
		if(hi == NULL)
			hi = fopen("playerSave.txt", "w+"); //OPEN THE DAMN FILE.
		fpos_t newPos;
		fgetpos(hi, &newPos);
		char c = fgetc(hi);
		while(c != EOF) //Get the amount of save Files within the playerSave.
		{
			if(c == '#')
				saveFiles++;
			c = fgetc(hi);
		}
		fsetpos(hi, &newPos);
		//I'm going to hell for this, aren't I?
		loadRects = new SRect[saveFiles];
		loadMessages = new TTtext[saveFiles];
		MyFont whoo;
		whoo.setFont(FONTSIZE); //My god, I feel like a retard.
		//For the save Files to be recognized, we need a Name, and a Level.
		for(int i = 0; i < saveFiles; i++)
		{
			//Start at the beginning, then move on to the next one.
			loadRects[i].setDimension(SPoint(200, 15));
			loadRects[i].setPosition(SPoint(100, (i*15)+100));
			//Save Files will be reconstructed: Name, Level, and then character P.
			char point[41];
			for(int lol = 0; lol < 40; lol++)
				point[lol] = ' ';
			point[40] = 0; //NULL TERMINATOR
			char playerLevel[4] = {' ', ' ', ' ', 0}; //Level variable. I don't feel like being descriptive atm.
			fscanf_s(hi, "%s", point,20); //This gets rid of the P
			fscanf_s(hi, "%s", point,20); //This is the actual NAME.
			strcat_s(point, ": Level ");
			fscanf_s(hi, "%s", playerLevel, 4);
			strcat_s(point, playerLevel);
			loadMessages[i].setFont(whoo.getFont());
			loadMessages[i].setMessage(point);

			//Now to get to the next player.
			while(true) //My god...I can't believe I'm actually using this.
			{
				c = fgetc(hi);
				if(c == '#')
					break;
			}
		}
		fclose(hi);
		//And that...should be everything.
	}
	void execute(baseEngine *be)
	{
		//DO NOTHING FOR NOW. COMPILE FOR ERROR CHECKING. No Errors...continue with execute function.
		//Input, Logic, Draw. Logic step...which requires an array of booleans. I...will not like this.
		
		if(stateUI != NULL)
		{
			for(int i = 0; i < saveFiles; i++)
			{
				if(loadRects[i].contains(SPoint(stateUI->getMouseX(), stateUI->getMouseY())) && stateUI->getClick() == 1)
				{
					checkClick = i;
				}
			}
		}
		SDL_Rect newRect;
		for(int i = 0; i < saveFiles; i++)
		{
			newRect.x = loadRects[i].x; newRect.y = loadRects[i].y; newRect.h = loadRects[i].h; newRect.w = loadRects[i].w;
			//cause of red flash @ load game
			SDL_FillRect(screen, &newRect, 0xff0000);
			loadMessages[i].printMessage(screen, loadRects[i].x, loadRects[i].y);
		}

		SDL_Flip(screen);
		if(checkClick >= 0)
			exit(be);
	}
	void exit(baseEngine *be)
	{
		SDL_FreeSurface(screen);
		if(optionOfReturn == -1)
			be->goToTitleScreen();
		else
		{
			delete [] loadRects; //I'm going to hell aren't I?
			delete [] loadMessages; //Yes. Yes you are.
			be->getPlayer()->loadPlayer(checkClick);
			be->changeState(actualGameState::instance());
		}
	}
	static loadGameState* instance() {static loadGameState instance; return &instance;}
	void handleInput(UserInput * obj) {stateUI = obj;}
};

class newGameState : public State
{
private:
	char playerName[21];
	char * enterNameMessage;
	UserInput * stateUI;
	SDL_Sprite newGameScreen;
	SDL_Surface * screen;
	bool typing, finished, shift;
	SRect type;
	SDL_Rect newType;
	int num;
	MyFont hi;
	TTtext playerNewName;
	TTtext enterNewMessage;
public:
	void enter(baseEngine *be) 
	{
		for(int i = 0; i < 20; i++)
		{
			playerName[i] = ' ';
		}
		playerName[20] = 0; //Null terminator.
		enterNameMessage = "Enter new name below: ";
		stateUI = NULL;
		screen = SDL_SCREEN_STARTUP;
		newGameScreen.setSprite("Sprites/newGameScreen.bmp", 626, 470, 0, 1);
		typing = finished = shift = false;
		type.x = newType.x = 219;
		type.y = newType.y = 224;
		type.h = newType.h = 43;
		type.w = newType.w = 204;
		num = 1;
		hi.setFont(FONTSIZE);
		playerNewName.setFont(hi.getFont());
		enterNewMessage.setFont(hi.getFont());
	}
	void execute(baseEngine *be) 
	{
		if(stateUI != NULL)
		{
			if(typing == true)
			{
				char c = stateUI->getLastKey();
				//char c = getch();
				if(c > 0)
				{
					if(c == 8 || c == 7) //Backspace, bell, tab, and newline.
						{
							num--;
							playerName[num] = ' ';
						}
					else if(c == 10 || c == 32 || c == 14 || c == 15 || c == 47 || c == 45)
					{
						//Do nothing.
					}
					else if(c == 13)
					{
						exit(be);
					}
					else if(num < 20)
					{
						playerName[num] = c;
						num++;
					}
				}
			}
			if(type.contains(SPoint(stateUI->getMouseX(), stateUI->getMouseY())) && stateUI ->getClick() == 1)
			{
				typing = true;
			}
		}
		playerNewName.setMessage(playerName);
		enterNewMessage.setMessage(enterNameMessage);
		//Draw
		newGameScreen.draw(screen, 0, 0);
		enterNewMessage.printMessage(screen, 100, 100);
		SDL_FillRect(screen, &newType , 0xff0000);
		playerNewName.printMessage(screen, type.x, type.y+15);
		SDL_Flip(screen);
	}
	void exit(baseEngine *be) 
	{
		SDL_FreeSurface(screen);
		be->getPlayer()->newGame();
		be->getPlayer()->setName(playerName);
		be->changeState(actualGameState::instance());
	}
	void handleInput(UserInput * obj) {stateUI = obj;}
	static newGameState* instance() {static newGameState instance; return &instance;}
};

class titleScreenState : public State
{
private:
	SRect newGame;
	SRect networkGame;
	SRect loadGame;
	SDL_Sprite titleScreen;
	int choice;
	SDL_Surface * screen2;
	UserInput * stateUI;
public:

	void enter(baseEngine* be) 
	{
		screen2 = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
		if(!titleScreen.isSprite())
			titleScreen.setSprite("Sprites/posTitleScreen.bmp", 626, 470, 0, 1); //MAGIC NUMBAHS = BAD
		choice = 0;
		newGame.setPosition(SPoint(68, 292));
		newGame.setDimension(SPoint(110, 137));
		networkGame.setDimension(SPoint(170, 117));
		networkGame.setPosition(SPoint(235, 58));
		loadGame.setDimension(SPoint(110, 137)); //These are debug numbers, btw. Not entirely accurate.
		loadGame.setPosition(SPoint(443, 306)); //Setting up the stuffs...
		//INCLUDE SDL STUFF HERE
		stateUI = NULL;
	}
	void execute(baseEngine* be) 
	{
		//Input Logic Draw
		if(stateUI != NULL)
		{
			if(newGame.contains(SPoint(stateUI->getMouseX(), stateUI->getMouseY())) && stateUI->getClick() == 1)
				choice = 1;
			else if(loadGame.contains(SPoint(stateUI->getMouseX(), stateUI->getMouseY()))&& stateUI->getClick() == 1)
				choice = 2;
			else if(networkGame.contains(SPoint(stateUI->getMouseX(), stateUI->getMouseY()))&& stateUI->getClick() == 1)
				choice = 3;
		}
		if(choice != 0)
			exit(be);

		SDL_FillRect(screen2, 0, SDL_MapRGB(screen2->format, 0, 0, 0));
		titleScreen.draw(screen2, 0, 0);
		SDL_Flip(screen2);
		SDL_Delay(SDLDELAY);
		if(stateUI != NULL)
			if(stateUI->getX())
				exit(be);
	}//Title screen stuffs.
	void exit(baseEngine* be) 
	{
		//Use the choice variable here, that's set during execute!
		SDL_FreeSurface(screen2);
		switch(choice)
		{
		case 1:
			be->changeState(newGameState::instance());
			break;
		case 2:
			be->changeState(loadGameState::instance());
			break;
		case 3:
			be->changeState(actualGameState::instance());
			break;
		default:
			break;
		}
		//IT SHOULD NEVER MAKE IT TO HERE. IF IT DOES, THEN SOMETHING IS WRONG.
		if(stateUI != NULL)
			if(stateUI->getX())
				SDL_Quit();
		enter(be);
	}
	static titleScreenState* instance() {static titleScreenState instance; return &instance;}
	void handleInput(UserInput * obj) {stateUI = obj;}
};

class splashScreenState : public State
{
public:
	void enter(baseEngine* be) {}
	void execute(baseEngine* be) {} //Timer or something similar.
	void exit(baseEngine* be) {be->changeState(titleScreenState::instance()); }
	static splashScreenState* instance() {static splashScreenState instance; return &instance;}
	void handleInput(UserInput * obj) {}
};

void baseEngine::goToTitleScreen()
{
	changeState(titleScreenState::instance());
	this->cState->enter(this);
}
#pragma once
#include "baseEngine.h"
#include "srect.h"
//A very simple state struct.

//SDL includes and defines
#define SDLDELAY		17
#define CHAT_MESSAGE_CAP	4
#define CHAT_CHARACTER_CAP	41
#include "sdl/sdl.h"
#include <stdio.h>
#include <conio.h>

//THIS NEXT LINE IS OMGWTFDANGEROUS, BE CAREFUL WHEN USING THIS LINE.
#define SDL_SCREEN_STARTUP			SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE)
//player sprite stuff
#define P_SPRITES					3
#define P_OFFSETS					4
#define P_X							300
#define	P_Y							100
#define	P_OFFX						150
#define	P_OFFY						250



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
		stateUI = NULL;
		then = SDL_GetTicks();
		now = passed = second = 0;
		myfps.setFont(FONTSIZE);
		fps.setFont(myfps.getFont());
		ifps = 0;
		fps.setMessage("0");
		be->getPlayer()->setGamePlayed(true);
		/*if(!be->getWorld()->getSuccess())
		{
			be->getWorld()->initWorld();
		}
		if(be->getPlayer()->getDeleted())
		{
			be->getPlayer()->initPlayer(be->getWorld());
		}*/
		be->getWorld()->setCamera(be->getPlayer()->getCamera());
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
			be->getWorld()->update(passed, be->getAH());
			be->getAH()->update(be->getWorld()->getCurrentWorld());
			be->getHUD()->updateHud(be->getPlayer(), stateUI, passed);
		}

		//SDL_FillRect(be->getScreen(), 0, SDL_MapRGB(be->getScreen()->format, 0, 0, 0));
		be->getWorld()->draw(be->getScreen());
		be->getHUD()->draw(be->getScreen());
		fps.printMessage(be->getScreen(), 0, 0);
		SDL_Flip(be->getScreen());
		if(stateUI != NULL)
			if(stateUI->getX())
				exit(be);
		//SDL_Delay(SDLDELAY);
	} //Input Logic Draw
	void exit(baseEngine* be) 
	{
		be->getPlayer()->destroyPlayer();
		be->getWorld()->destroyWorld();
		be->goToTitleScreen();
	}
	static actualGameState* instance() {static actualGameState instance; return &instance;}
	void handleInput(UserInput * obj) {stateUI = obj;}
};

class clientGameState: public State
{
private:
	UserInput * stateUI;
	Client cClient;
	Uint32 then, now, passed, second;
	TTtext fps;
	MyFont myfps;
	char cfps[10];
	int ifps;
	char chatMessages[CHAT_MESSAGE_CAP][CHAT_CHARACTER_CAP]; //This can be changed easily.
	TTtext chatLog[CHAT_MESSAGE_CAP];
	TTtext chatLogToSend;
	char chatMessageToSend[CHAT_CHARACTER_CAP]; //40 character cap.
	bool typing;
	int num;
public:
	void enter(baseEngine *be) 
	{
		num = 0;
		bool lolz = cClient.startClient();
		typing = false;
		if(!lolz)
		{
			printf("WinSock failed. Returning to Title Screen.");
			be->goToTitleScreen();
		}
		else
		{
			cClient.setWorld(be->getWorld());
			lolz = cClient.connectToServer("127.0.0.1");
			if(!lolz)
			{
				printf("Could not connect. Return to Title Screen.");
				be->goToTitleScreen();
			}
		}
		stateUI = NULL;
		then = SDL_GetTicks();
		now = passed = second = 0;
		myfps.setFont(FONTSIZE);
		for(int i = 0; i < CHAT_MESSAGE_CAP; i++)
		{
			chatLog[i].setFont(myfps.getFont());
			for(int k = 0; k < CHAT_CHARACTER_CAP; k++)
			{
				chatMessages[i][k] = ' ';
			}
		}
		for(int i = 0; i < CHAT_CHARACTER_CAP; i++)
			chatMessageToSend[i] = ' ';
		chatLogToSend.setFont(myfps.getFont());
		fps.setFont(myfps.getFont());
		ifps = 0;
		fps.setMessage("0");
		be->getPlayer()->setGamePlayed(true);
		if(!be->getWorld()->getSuccess())
		{
			be->getWorld()->initWorld();
		}
		if(!be->getPlayer()->getGamePlayed())
		{
			be->getPlayer()->initPlayer(be->getWorld());
			if(!be->getPlayer()->loadPlayer(0)) //Force the player to be the first game in their list. We can change this later.
				be->getPlayer()->newGame(); //If they don't HAVE a game...then create one.
		}
	}
	void execute(baseEngine *be) 
	{
		cClient.run();
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
			if(typing)
			{
				char c = stateUI->getLastKey();
				//char c = getch();
				if(c > 0)
				{
					if(c == 8 || c == 7) //Backspace, bell, tab, and newline.
						{
							num--;
							chatMessageToSend[num] = ' ';
						}
					else if(c == 10 || c == 14 || c == 15 || c == 47 || c == 45)
					{
						//Do nothing.
					}
					else if(num < 40) //This one HAS TO BE lower than 15. I'm going to assume for now that the user KNOWS WHAT THEY'RE DOING.
					{
						chatMessageToSend[num] = c;
						num++;
					}
				}
				if(c == 13)
				{
					cClient.sendMessage(chatMessageToSend);
					num = 0;
					for(int i = 0; i < CHAT_CHARACTER_CAP; i++)
						chatMessageToSend[i] = ' ';
					typing = false;
				}
			}
			else
			{
				char newC = stateUI->getLastKey();
				be->getPlayer()->handleInput(stateUI, be->getWorld(), be->getAH());
				if(newC == 13)
					typing = true;
			}
			be->getWorld()->update(passed, be->getAH());
			be->getAH()->update(be->getWorld()->getCurrentWorld());
			be->getHUD()->updateHud(be->getPlayer(), stateUI, passed);
		}

		//SDL_FillRect(be->getScreen(), 0, SDL_MapRGB(be->getScreen()->format, 0, 0, 0));
		be->getWorld()->draw(be->getScreen());
		be->getHUD()->draw(be->getScreen());
		fps.printMessage(be->getScreen(), 0, 0);

		//Insert chat Stuff here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if(typing)
		{
			chatLogToSend.setMessage(chatMessageToSend);
			chatLogToSend.printMessage(be->getScreen(), SCREEN_WIDTH-100, SCREEN_HEIGHT - 50);
		}
		if(cClient.isChatMessageWaiting())
		{
			//There is a chat message waiting. Get it after waiting all this stuff...
			for(int i = 3; i > 0; i--)
			{
				for(int k = 0; k < CHAT_CHARACTER_CAP; k++)
					chatMessages[i][k] = chatMessages[i-1][k];
			}
			char * lololol = cClient.getChat();
			for(int k = 0; k < CHAT_CHARACTER_CAP; k++)
				chatMessages[0][k] = lololol[k];
			cClient.clearChat();
		}
		for(int i = 0; i < 4; i++)
		{
			chatLog[i].setMessage(chatMessages[i]);
			chatLog[i].printMessage(be->getScreen(), SCREEN_WIDTH-150, SCREEN_HEIGHT-((i*15)+80));
		}

		SDL_Flip(be->getScreen());
		if(stateUI != NULL)
			if(stateUI->getX())
				exit(be);
	//	SDL_Delay(SDLDELAY);
	}
	void exit(baseEngine *be) 
	{
		cClient.shutDown();
	}
	static clientGameState * instance() {static clientGameState instance; return &instance;}
	void handleInput(UserInput * obj) {stateUI = obj;}
};

class hostGameState: public State
{
private:

	//Basic State stuff
	UserInput * stateUI;
	Server hostServer;
	TemplateVector2<Player*> listOfPlayers;
	Uint32 then, now, passed, second;

	//FPS stuff
	TTtext fps;
	MyFont myfps;
	char cfps[10];
	int ifps;

	//Chat stuff
	//FUCK YOU STRING. FUCK YOU AND EVERYTHING YOU STAND FOR.
	char chatMessages[CHAT_MESSAGE_CAP][CHAT_CHARACTER_CAP];
	char * messageReceiver;
	bool typing;
	TTtext chatLog[CHAT_MESSAGE_CAP];
	TTtext chatToSend;
	char currentMessageToSend[CHAT_CHARACTER_CAP];
	int num;

public:
	void enter(baseEngine *be) 
	{
		//Starting the server stuff

		printf("Attempting to start server.\n");
		bool serverRunning = hostServer.startServer();
		printf("startServer has been passed. Success is 0. %d", serverRunning);
		if(!serverRunning)
		{
			printf("Failed. Returning to title screen.");
			be->goToTitleScreen();
		}
		else //The server starting was successful.
		{
			printf("Server is successul.\n");
			hostServer.changeWorld(be->getWorld());
		}
		//Ending the server stuff, starting FPS

		stateUI = NULL;
		then = SDL_GetTicks();
		now = passed = second = 0;
		myfps.setFont(FONTSIZE);

		//Ending FPS. Starting Chat.
		for(int k = 0; k < CHAT_CHARACTER_CAP; k++)
		{
			currentMessageToSend[k] = ' ';
		}
		for(int i = 0; i < CHAT_MESSAGE_CAP; i++)
		{
			chatLog[i].setFont(myfps.getFont());
			for(int k = 0; k < CHAT_CHARACTER_CAP; k++)
			{
				chatMessages[i][k] = ' ';
			}
		}
		chatToSend.setFont(myfps.getFont());
		messageReceiver = NULL;
		//Ending chat, finishing up FPS
		fps.setFont(myfps.getFont());
		ifps = 0;
		fps.setMessage("0");
		//Engine game played is set to true, which will tell the game to save.
		be->getPlayer()->setGamePlayed(true);
		//Initializes the world and player, if it was ESCed out earlier.
		if(!be->getWorld()->getSuccess())
		{
			be->getWorld()->initWorld();
		}
		if(!be->getPlayer()->getGamePlayed())
		{
			be->getPlayer()->initPlayer(be->getWorld());
		}
		//Setting the final chat things to false and 0.
		typing = false;
		num = 0;
	}
	void execute(baseEngine *be) 
	{
//		printf("Server is running.\n");
		hostServer.run();
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
			if(typing)
			{
				char c = stateUI->getLastKey();
				//char c = getch();
				if(c > 0)
				{
					if(c == 8 || c == 7) //Backspace, bell, tab, and newline.
					{
						num--;
						currentMessageToSend[num] = ' ';
					}
					else if(c == 10 || c == 14 || c == 15 || c == 47 || c == 45)
					{
						//Do nothing.
					}
					else if(num < CHAT_CHARACTER_CAP)
					{
						currentMessageToSend[num] = c;
						num++;
					}
				}
				//If typing is already active, and the enter key was pressed.
				if(c == 13)
				{
					hostServer.sendMessage(currentMessageToSend);
					num = 0;
					typing = false;
					for(int i = 3; i > 0; i--)
					{
						for(int k = 0; k < CHAT_CHARACTER_CAP; k++)
						{
							chatMessages[i][k] = chatMessages[i-1][k];
						}
					}
					//Get the currentMessageToSend and...ECHO it back to the player.
					for(int i = 0; i < CHAT_CHARACTER_CAP; i++)
					{
						chatMessages[0][i] = currentMessageToSend[i];
						currentMessageToSend[i] = ' ';
					}
				}
			}
			else
			{
				char c = stateUI->getLastKey();
				be->getPlayer()->handleInput(stateUI, be->getWorld(), be->getAH());
				if(c == 13)
					typing = true;
			}
			//Update the world, audiohandler, and HUD.
			be->getWorld()->update(passed, be->getAH());
			be->getAH()->update(be->getWorld()->getCurrentWorld());
			be->getHUD()->updateHud(be->getPlayer(), stateUI, passed);
		}
		//SDL_FillRect(be->getScreen(), 0, SDL_MapRGB(be->getScreen()->format, 0, 0, 0));
		be->getWorld()->draw(be->getScreen());
		be->getHUD()->draw(be->getScreen());
		fps.printMessage(be->getScreen(), 0, 0);

		//Input chat system stuff here!!!!!!
		if(typing)
		{
			chatToSend.setMessage(currentMessageToSend);
			chatToSend.printMessage(be->getScreen(), SCREEN_WIDTH-100, SCREEN_HEIGHT-50);
		}

		if(hostServer.getMessage(messageReceiver))
		{
			for(int i = 3; i > 0; i--)
			{
				for(int k = 0; k < CHAT_CHARACTER_CAP; k++)
					chatMessages[i][k] = chatMessages[i-1][k];
			}
			for(int i = 0; i < CHAT_CHARACTER_CAP; i++)
				chatMessages[0][i] = messageReceiver[i];
		}

		for(int i = 0; i < CHAT_MESSAGE_CAP; i++)
		{
			chatLog[i].setMessage(chatMessages[i]);
			if(chatLog[i].messageAvailable())
				chatLog[i].printMessage(be->getScreen(), SCREEN_WIDTH-150, SCREEN_HEIGHT-((i*15)+80));
		}
		SDL_Flip(be->getScreen());
		if(stateUI != NULL)
			if(stateUI->getX())
				exit(be);
	//	SDL_Delay(SDLDELAY);
	}
	void exit(baseEngine *be) 
	{
		hostServer.shutDown();
	}
	static hostGameState* instance() {static hostGameState instance; return &instance;}
	void handleInput(UserInput * obj) {stateUI = obj;}
};
class networkStartState : public State
{
private:
	UserInput * stateUI;
	SDL_Rect hostGame;
	SDL_Rect connectGame;
	SDL_Rect enterIP;
	TTtext hostAGame;
	TTtext connectAGame;
	TTtext enterIPAddr;
	MyFont myfont;
	int choiceOfGame;
	SRect cGame;
	SRect hGame; //Not that h.
	SRect ipEnterBox;
	int num;
	bool exitToNextState, typing;
	char ipEnter[15]; //The IP the user Enters. Something new I found out: You can have extra 0's and spaces and it won't matter.
public:
	void enter(baseEngine *be)
	{
		stateUI = NULL;
		hostGame.x = 100; hostGame.y = 100; hostGame.w = 100; hostGame.h = 100;
		connectGame.x = 400; connectGame.y = 100; connectGame.w = 200; connectGame.h = 100;
		enterIP.x = 400; enterIP.y = 215; enterIP.w = 200; enterIP.h = 40;
		myfont.setFont(20);
		hostAGame.setFont(myfont.getFont());
		connectAGame.setFont(myfont.getFont());
		enterIPAddr.setFont(myfont.getFont());
		hostAGame.setMessage("Host Game");
		connectAGame.setMessage("Connect To A Game");
		choiceOfGame = 0;
		cGame.set(connectGame);
		hGame.set(hostGame);
		ipEnterBox.set(enterIP);
		exitToNextState = typing = false;
		for(int i = 0; i < 15; i++)
		{
			ipEnter[i] = ' ';
		}
		num = 0;
	}
	void execute(baseEngine *be)
	{
		if(stateUI != NULL)
		{
			if(typing)
			{
				char c = stateUI->getLastKey();
				//char c = getch();
				if(c > 0)
				{
					if(c == 8 || c == 7) //Backspace, bell, tab, and newline.
						{
							num--;
							ipEnter[num] = ' ';
						}
					else if(c == 10 || c == 32 || c == 14 || c == 15 || c == 47 || c == 45)
					{
						//Do nothing.
					}
					else if(num < 15) //This one HAS TO BE lower than 15. I'm going to assume for now that the user KNOWS WHAT THEY'RE DOING.
					{
						ipEnter[num] = c;
						num++;
					}
				}
			}
			
			if(cGame.contains(SPoint(stateUI->getMouseX(), stateUI->getMouseY())) && stateUI->getClick() == 1)
			{
				choiceOfGame = 2;
				exitToNextState = true;
			}
			if(hGame.contains(SPoint(stateUI->getMouseX(), stateUI->getMouseY())) && stateUI->getClick() == 1)
			{
				choiceOfGame = 1;
				exitToNextState = true;
			}
			if(stateUI->getX())
				exitToNextState = true;
			if(ipEnterBox.contains(SPoint(stateUI->getMouseX(), stateUI->getMouseY())) && stateUI->getClick() == 1)
				typing = true;
			else if(stateUI->getClick() == 1) //If the user clicks OUTSIDE the typing box.
				typing = false;
		}
		//That was input/logic. Now its draw.
		
		enterIPAddr.setMessage(ipEnter);

		SDL_FillRect(be->getScreen(), &connectGame, 0xffffff); //I think that's white...?
		SDL_FillRect(be->getScreen(), &hostGame, 0xffffff);
		SDL_FillRect(be->getScreen(), &enterIP, 0xffffff);
		hostAGame.printMessage(be->getScreen(), 100, 100);
		connectAGame.printMessage(be->getScreen(), 400, 100);
		enterIPAddr.printMessage(be->getScreen(), 400, 215);

		SDL_Flip(be->getScreen());
		if(exitToNextState)
		{
			exit(be);
		}
	}
	void exit(baseEngine *be)
	{
		exitToNextState = false;
		switch(choiceOfGame)
		{
		case 1:
			//Host Game
			be->changeState(hostGameState::instance());
			break;
		case 2:
			be->changeState(clientGameState::instance());
			//Connect Game
			break;
		case 0:
			be->goToTitleScreen();
			//...Break out.
			break;
		}
		//choiceOfGame = 0;
//		be->goToTitleScreen();
	}
	static networkStartState* instance() {static networkStartState instance; return &instance;}
	void handleInput(UserInput * obj) {stateUI = obj;}
};

class loadGameState: public State
{
private:
	UserInput * stateUI;	//The UserInput. We all know how this goes.
//	SDL_Surface * screen;	//The SDL_Surface of which this State writes to.
	int optionOfReturn;		//The erroring integer, or the saveFile that the user has selected.
	int saveFiles;			//The number of Save Files located in the file.
	SRect * loadRects;		//...The soon to be array of rectangles that allow the user to switch which save file.
	TTtext playerMessages;	//Any messages that may need to occur, such as erroring.
	TTtext * loadMessages;	//I'm going to go to hell for this, aren't I?
	int checkClick;
public:
	void enter(baseEngine *be)
	{
	//	screen = SDL_SCREEN_STARTUP;
		optionOfReturn = 0; //Initialize the erroring integer.
		checkClick = -1;
		saveFiles = 0;
		FILE * hi;
		fopen_s(&hi, "playerSave.txt", "r"); //Open the file for reading.
		//if(hi == NULL)
		//{
		//	optionOfReturn = -1; //Go back to Title Screen if the FILE cannot be opened, for some odd weird reason.
		//	exit(be);
		//}
		if(hi == NULL)
			fopen_s(&hi, "playerSave.txt", "w+"); //OPEN THE DAMN FILE.
		fpos_t newPos;
		fgetpos(hi, &newPos);
		char c = fgetc(hi);
		while(c != EOF) //Get the amount of save Files within the playerSave.
		{
			if(c == '#')
				if((c = fgetc(hi)) != '#')
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
			char point[CHAT_CHARACTER_CAP];
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
			if(stateUI->getX())
			{
				optionOfReturn = -1;
				checkClick = 1;
			}
		}
		SDL_Rect newRect;
		for(int i = 0; i < saveFiles; i++)
		{
			newRect.x = loadRects[i].x; newRect.y = loadRects[i].y; newRect.h = loadRects[i].h; newRect.w = loadRects[i].w;
			//cause of red flash @ load game
			SDL_FillRect(be->getScreen(), &newRect, 0xff0000);
			loadMessages[i].printMessage(be->getScreen(), loadRects[i].x, loadRects[i].y);
		}

		SDL_Flip(be->getScreen());
		if(checkClick >= 0)
			exit(be);
	}
	void exit(baseEngine *be)
	{
		//SDL_FreeSurface(screen);
		if(optionOfReturn == -1)
			be->goToTitleScreen();
		else
		{
			delete [] loadRects; //I'm going to hell aren't I?
			delete [] loadMessages; //Yes. Yes you are.
			if(be->getPlayer()->getDeleted())
			{
				be->getWorld()->initWorld();
				be->getPlayer()->initPlayer(be->getWorld());
			}
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
	SDL_Sprite newGameScreen, playerSprites[P_SPRITES];
	//SDL_Surface * screen;
	bool typing, finished, shift, drawBoarder;
	int num;
	e_classType p_num;
	SRect type , boarder, playerArea[P_SPRITES];
	SDL_Rect newType;
	MyFont hi;
	TTtext playerNewName;
	TTtext enterNewMessage;
	bool exitToTitle;
public:
	void enter(baseEngine *be) 
	{
		for(int i = 0; i < 20; i++)
		{
			playerName[i] = ' ';
		}
		playerName[20] = 0; //Null terminator.
		enterNameMessage = "Enter new name below: ";
		drawBoarder = false;
		stateUI = NULL;
	//	screen = SDL_SCREEN_STARTUP;
		newGameScreen.setSprite("Sprites/newGameScreen.bmp", 626, 470, 0, 1);
		char temp[32];
		for(int i = 0;  i < P_SPRITES; i++)
		{
			sprintf_s(temp, "Sprites/player%i.bmp", i);
			playerSprites[i].setSprite(temp, P_WIDTH, P_HEIGHT, FRAME_RATE, NUM_ROWS);
			playerSprites[i].setTransparency(COLOR_TRANSPARENT);
			
			playerSprites[i].stretch(200, 200);
			playerArea[i].set(P_X, P_Y, playerSprites[i].getWidth(), playerSprites[i].getHeight());
		}
		boarder.h = playerSprites[0].getHeight() + P_OFFSETS;
		boarder.w = playerSprites[0].getWidth() + P_OFFSETS;
		//force the other 2 at different locations
		playerArea[1].setPosition(SPoint(P_X - P_OFFX, P_Y + P_OFFY));
		playerArea[2].setPosition(SPoint(P_X + P_OFFX, P_Y + P_OFFY));

		typing = finished = exitToTitle = shift = false;
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
			if(stateUI->getX())
				exitToTitle = true;
			if(stateUI->getClick())
			{
				for(int i = 0; i < P_SPRITES; i++)
				{
					if(playerArea[i].contains(SPoint(stateUI->getMouseX(), stateUI->getMouseY())))
					{
						drawBoarder = true;
						boarder.setPosition(SPoint(playerArea[i].getX() - (P_OFFSETS/2), playerArea[i].getY() - (P_OFFSETS/2)));
						p_num = (e_classType)i;
					}
				}
			}
		}
		playerNewName.setMessage(playerName);
		enterNewMessage.setMessage(enterNameMessage);
		//Draw
		newGameScreen.draw(be->getScreen(), 0, 0);
		enterNewMessage.printMessage(be->getScreen(), 100, 100);
		SDL_FillRect(be->getScreen(), &newType , 0xff0000);
		playerNewName.printMessage(be->getScreen(), type.x, type.y+15);
		if(drawBoarder)
		{
			SDL_FillRect(be->getScreen(), &boarder, 0xffffff);
		}
		for(int i = 0; i < P_SPRITES; i++)
		{
			playerSprites[i].setRIndex(2);
			playerSprites[i].draw(be->getScreen(), playerArea[i].getX(),playerArea[i].getY()); 
		}
		SDL_Flip(be->getScreen());
		if(exitToTitle == true && drawBoarder)
			exit(be);
	}
	void exit(baseEngine *be) 
	{
	//	SDL_FreeSurface(screen);
		for(int i = 0;  i < P_SPRITES; i++)
			playerSprites[i].stretch(50, 50);
		if(exitToTitle == false)
		{
			if(be->getPlayer()->getDeleted())
			{
				be->getWorld()->initWorld();
				be->getPlayer()->initPlayer(be->getWorld());
			}
			be->getPlayer()->newGame();
			be->getPlayer()->setName(playerName);
			be->getPlayer()->initSprite(&playerSprites[p_num]);
			be->getPlayer()->setSpriteNum(p_num);
			be->changeState(actualGameState::instance());
		}
		else
			be->goToTitleScreen();
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
	//SDL_Surface * screen2;
	UserInput * stateUI;
public:

	void enter(baseEngine* be) 
	{
		//screen2 = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
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

		SDL_FillRect(be->getScreen(), 0, SDL_MapRGB(be->getScreen()->format, 0, 0, 0));
		titleScreen.draw(be->getScreen(), 0, 0);
		SDL_Flip(be->getScreen());
		SDL_Delay(SDLDELAY);
		if(stateUI != NULL)
			if(stateUI->getX())
				exit(be);
	}//Title screen stuffs.
	void exit(baseEngine* be) 
	{
		//Use the choice variable here, that's set during execute!
	//	SDL_FreeSurface(screen2);
		switch(choice)
		{
		case 1:
			be->changeState(newGameState::instance());
			break;
		case 2:
			be->changeState(loadGameState::instance());
			break;
		case 3:
			be->changeState(networkStartState::instance());
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
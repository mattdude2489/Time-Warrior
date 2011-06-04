//UserInput Class.
//Made on 3/5/11, edited on 4/1/11
//Made by Matt Morrill, edited last by Matt Morrill
#pragma once //Almost forgot this!


enum e_mouseClick	{CLICK_NONE, CLICK_LEFT, CLICK_MIDDLE, CLICK_RIGHT};
enum e_keyboardKey	{KEY_NONE = 'n', KEY_UP = 'w', KEY_LEFT = 'a', KEY_DOWN = 's', KEY_RIGHT = 'd', KEY_HOT_ATK1_BAS = '1', KEY_HOT_ATK1_ADV = '2', KEY_HOT_ATK1_EXP = '3', KEY_HOT_ATK1_LEG = '4' ,KEY_HOT_ATK2_BAS = '5', KEY_HOT_ATK2_ADV = '6', KEY_HOT_ATK2_EXP = '7', KEY_HOT_ATK2_LEG = '8', KEY_TRUE_NONE = 10};

class UserInput
{
	private:
		int mouseX, mouseY;		//Mouse's x and y position on screen.
		int mouseClick;			//Whether the mouse is clicked or not.
		char keyPressUpDown;	//If the user is pressing up or down.
		char keyPressLeftRight;	//If the user is pressing left or right.
		char initKey;			//The initial key. All input gets passed through this.
		char lastKeyPressed;	//The last key pressed. Similar to initKey, but stores it all the time.
		char hotKeyLeft;		//The hot key pressed for the left mouse click.
		char hotKeyRight;		//The hot key pressed for the right mouse click.
		bool spaceBar;			//The spaceBar. When pressed, activate NPC dialogue.
		bool shift;				//The shift key. When pressed, minus 32 to get the capital letter.
		bool esc;				//The escape key. Will close the program if put in full-screen.

	public:
		UserInput():mouseX(0), mouseY(0), mouseClick(CLICK_NONE), keyPressUpDown(KEY_NONE), keyPressLeftRight(KEY_NONE), initKey(KEY_NONE), esc(false), spaceBar(false), shift(false){} //Basic constructor.
		void setMouse(int x, int y) {mouseX = x; mouseY = y;}
		int getMouseX() {return mouseX;}
		int getMouseY() {return mouseY;}
		int getMouseXY(int &cX, int &cY) {cX = mouseX; cY = mouseY;} //So you DON'T have to use just getMouseX and getMouseY.
		//Sets and gets. And there's a lot of sets and gets.
		void setClick(int click) {mouseClick = click;}
		int getClick() {return mouseClick;}
		void setKeyUD(char key) {keyPressUpDown = key;}
		char getKeyUD() {return keyPressUpDown;}
		void setKeyLR(char key) {keyPressLeftRight = key;}
		char getKeyLR() {return keyPressLeftRight;}
		void setHKeyL(char key) {hotKeyLeft = key;}
		void setHKeyR(char key) {hotKeyRight = key;}
		char getHKeyL() {return hotKeyLeft;}
		char getHKeyR() {return hotKeyRight;}
		void setKey(char key) {initKey = key;}
		void setLastKey(char key) {lastKeyPressed = key;}
		char getKey() {return initKey;}
		bool getSpace() {return spaceBar;}
		//Resetting the mouse. Used in order to prevent the mouse being held down.
		void resetClick() {mouseClick = CLICK_NONE;}
		bool getX() {bool cEsc = esc; esc = false; return cEsc;}
		char getLastKey() 
		{
			char lastKey;
			lastKey = lastKeyPressed; 
			lastKeyPressed = 10; 
			if(shift) 
				lastKey-=32;
			if(lastKey < 32 && shift)
				lastKey = 10;
			return lastKey;
		}
		
		//If the initKey is a W, A, S, or D, then set it to keyPressUpDown or keyPressLeftRight.
		void updateUI(bool upDown)
		{
			if(upDown) //If the value is 0, false, then the key is up. UNSET the values.
			{
				if(initKey != KEY_NONE)
				{
					if(initKey == KEY_UP || initKey == KEY_DOWN)
						keyPressUpDown = KEY_NONE;
					if(initKey == KEY_LEFT || initKey == KEY_RIGHT)
						keyPressLeftRight = KEY_NONE;
					if(initKey == KEY_HOT_ATK1_BAS || initKey == KEY_HOT_ATK1_ADV || initKey == KEY_HOT_ATK1_EXP || initKey == KEY_HOT_ATK1_LEG)
						hotKeyLeft = KEY_NONE;
					if(initKey == KEY_HOT_ATK2_BAS || initKey == KEY_HOT_ATK2_ADV || initKey == KEY_HOT_ATK2_EXP || initKey == KEY_HOT_ATK2_LEG)
						hotKeyRight = KEY_NONE;
					if(initKey == ' ')
						spaceBar = false;
					if(initKey == 48 || initKey == 47)
						shift = false;
					if(!(initKey == 'm' || initKey == 'i'))
						initKey = KEY_NONE; //Reset the initial Key.
				}
			}
			else
				//Set the values.
				lastKeyPressed = initKey;
				if(initKey != KEY_NONE)
				{
					if(initKey == KEY_UP || initKey == KEY_DOWN)
						keyPressUpDown = initKey;
					if(initKey == KEY_LEFT || initKey == KEY_RIGHT)
						keyPressLeftRight = initKey;
					if(initKey == KEY_HOT_ATK1_BAS || initKey == KEY_HOT_ATK1_ADV || initKey == KEY_HOT_ATK1_EXP || initKey == KEY_HOT_ATK1_LEG)
						hotKeyLeft = initKey;
					if(initKey == KEY_HOT_ATK2_BAS || initKey == KEY_HOT_ATK2_ADV || initKey == KEY_HOT_ATK2_EXP || initKey == KEY_HOT_ATK2_LEG)
						hotKeyRight = initKey;
					if(initKey == ' ')
						spaceBar = true;
					if(initKey == 27)
						esc = true;
					if(initKey == 47 || initKey == 48)
						shift = true;
					if(!(initKey == 'm' || initKey == 'i'))
						initKey = KEY_NONE; //Reset the initial Key.
				}
		}
	/*	void sendUi2Server(char a_str[])
		{
			sprintf(a_str, "%c%c%i%03i%03i", keyPressLeftRight, keyPressUpDown, mouseClick, mouseX, mouseY );
		}*/
		bool keysChanged(char a_in[])
		{
			if(keyPressLeftRight == a_in[0]){
				if(keyPressUpDown == a_in[1]){
					return true;}
			}

			return false;
		}
		//void convertServerInfo(char * a_in)
		//{
		//	char buffer[10];
		//	keyPressLeftRight = a_in[0];
		//	keyPressUpDown = a_in[1];
		//	buffer[0] = a_in[2];
		//	mouseClick = atoi(buffer);
		//	sprintf_s(buffer, "%c%c%c", a_in[3],a_in[4],a_in[5]);
		//	mouseX = atoi(buffer);
		//	sprintf_s(buffer, "%c%c%c", a_in[6],a_in[7],a_in[8]);
		//	mouseY = atoi(buffer);
		//}
};
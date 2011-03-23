//UserInput Class.
//Made on 3/5/11, edited on 3/10/11
//Made by Matt Morrill, edited last by Matt Morrill
#pragma once //Almost forgot this!

enum e_mouseClick {CLICK_NONE, CLICK_LEFT, CLICK_MIDDLE, CLICK_RIGHT};
enum e_keyboardKey {KEY_NONE = 'n', KEY_UP = 'w', KEY_LEFT = 'a', KEY_DOWN = 's', KEY_RIGHT = 'd', KEY_HOT_ATK1_BAS = '1', KEY_HOT_ATK1_ADV = '2', KEY_HOT_ATK1_EXP = '3', KEY_HOT_ATK1_LEG = '4' ,KEY_HOT_ATK2_BAS = '5', KEY_HOT_ATK2_ADV = '6', KEY_HOT_ATK2_EXP = '7', KEY_HOT_ATK2_LEG = '8'};

class UserInput
{
	private:
		int mouseX, mouseY;		//Mouse's x and y position on screen.
		int mouseClick;			//Whether the mouse is clicked or not.
		char keyPressUpDown;	//If the user is pressing up or down.
		char keyPressLeftRight;	//If the user is pressing left or right.
		char initKey;			//The initial key. All input gets passed through this.
		char hotKeyLeft;		//The hot key pressed for the left mouse click.
		char hotKeyRight;		//The hot key pressed for the right mouse click.

	public:
		UserInput():mouseX(0), mouseY(0), mouseClick(CLICK_NONE), keyPressUpDown(KEY_NONE), keyPressLeftRight(KEY_NONE), initKey(KEY_NONE){} //Basic constructor.
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
		char getKey() {return initKey;}
		//Resetting the mouse. Used in order to prevent the mouse being held down.
		void resetClick() {mouseClick = CLICK_NONE;}
		
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
					if(!(initKey == 'm' || initKey == 'i'))
						initKey = KEY_NONE; //Reset the initial Key.
				}
			}
			else
				//Set the values.
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
					if(!(initKey == 'm' || initKey == 'i'))
						initKey = KEY_NONE; //Reset the initial Key.
				}
		}
};
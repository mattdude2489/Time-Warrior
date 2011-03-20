//UserInput Class.
//Made on 3/5/11, edited on 3/10/11
//Made by Matt Morrill, edited last by Matt Morrill
#pragma once //Almost forgot this!

enum e_mouseClick {CLICK_NONE, CLICK_LEFT, CLICK_MIDDLE, CLICK_RIGHT};
enum e_keyboardKey {KEY_NONE = 'n', KEY_UP = 'w', KEY_LEFT = 'a', KEY_DOWN = 's', KEY_RIGHT = 'd'};

class UserInput
{
	private:
		int mouseX, mouseY;
		int mouseClick;
		char keyPressUpDown;
		char keyPressLeftRight;
		char initKey;

	public:
		UserInput():mouseX(0), mouseY(0), mouseClick(CLICK_NONE), keyPressUpDown(KEY_NONE), keyPressLeftRight(KEY_NONE), initKey(KEY_NONE){} //Basic constructor.
		void setMouse(int x, int y) {mouseX = x; mouseY = y;} 
		int getMouseX() {return mouseX;}
		int getMouseY() {return mouseY;}
		int getMouseXY(int &cX, int &cY) {cX = mouseX; cY = mouseY;} //So you DON'T have to use just getMouseX and getMouseY.
		void setClick(int click) {mouseClick = click;}
		int getClick() {return mouseClick;}
		void setKeyUD(char key) {keyPressUpDown = key;}
		char getKeyUD() {return keyPressUpDown;}
		void setKeyLR(char key) {keyPressLeftRight = key;}
		char getKeyLR() {return keyPressLeftRight;}
		void setKey(char key) {initKey = key;}
		char getKey() {return initKey;}
		void resetClick() {mouseClick = 0;}
		
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
					if(!(initKey == 'm' || initKey == 'i'))
						initKey = KEY_NONE; //Reset the initial Key.
				}
		}
};
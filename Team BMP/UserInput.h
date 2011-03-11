//UserInput Class.
//Made on 3/5/11, edited on 3/10/11
//Made by Matt Morrill, edited last by Matt Morrill
#pragma once //Almost forgot this!

class UserInput
{
	private:
		int mouseX, mouseY;
		int mouseClick; //0 is no clicks, 1 is left click, 2 is right click
		char keyPressUpDown;
		char keyPressLeftRight;
		char initKey;

	public:
		UserInput():mouseX(0), mouseY(0), mouseClick(0), keyPressUpDown('n'), keyPressLeftRight('n'), initKey('n'){} //Basic constructor.
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
		
		//If the initKey is a W, A, S, or D, then set it to keyPressUpDown or keyPressLeftRight.
		void updateUI(bool upDown)
		{
			if(upDown) //If the value is 0, false, then the key is up. UNSET the values.
			{
				if(initKey != 'n')
				{
					if(initKey == 'w' || initKey == 's')
						keyPressUpDown = 'n';
					if(initKey == 'a' || initKey == 'd')
						keyPressLeftRight = 'n';
					if(!(initKey == 'm' || initKey == 'i'))
						initKey = 'n'; //Reset the initial Key.
				}
			}
			else
				//Set the values.
				if(initKey != 'n')
				{
					if(initKey == 'w' || initKey == 's')
						keyPressUpDown = initKey;
					if(initKey == 'a' || initKey == 'd')
						keyPressLeftRight = initKey;
					if(!(initKey == 'm' || initKey == 'i'))
						initKey = 'n'; //Reset the initial Key.
				}
		}
};
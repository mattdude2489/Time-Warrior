//UserInput Class.
//Made on 3/5/11, edited on 3/5/11
//Made by Matt Morrill, edited last by Matt Morrill

class UserInput
{
	private:
		int mouseX, mouseY;
		int mouseClick; //0 is no clicks, 1 is left click, 2 is right click
		char * keyPress;

	public:
		UserInput():mouseX(0), mouseY(0), mouseClick(0){}
		void setMouse(int x, int y) {mouseX = x; mouseY = y;}
		int getMouseX() {return mouseX;}
		int getMouseY() {return mouseY;}
		int getMouseXY(int &cX, int &cY) {cX = mouseX; cY = mouseY;}
		void setClick(int click) {mouseClick = click;}
		int getClick() {return mouseClick;}
		void setKey(char * key) {keyPress = key;}
		char * getKey() {return keyPress;}
};
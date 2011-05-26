//Hi there. This is the NPC class.
//Created by Matt Morrill, edited last by Matt Morrill (who actually uses these?)

#pragma once
#include "trueTextPrinter.h"
#include "Entity.h"
#include "srect.h"

class NonPlayerChar : public Entity
{
private:
	char * whatTheyHaveToSay; //Probably nothing worthwhile.
	TTtext m_Text[3]; //The text variable.
	MyFont m_arialFont;
	bool drawText;
public:
	NonPlayerChar(SDL_Sprite * a_sprite)
		:Entity(a_sprite){initNPC();}
	NonPlayerChar(char* talk, SDL_Sprite *a_sprite)
		:Entity(a_sprite)
	{
		initNPC(talk);
	}
	void initNPC()
	{
		initNPC("Hi.");
	}
	void initNPC(char * talk)
	{
		m_eType = NPC;
		whatTheyHaveToSay = talk;
		drawText = false;
		m_arialFont.changeSizeTo(20);
		for(int i = 0; i < 3; i++)
		{
			m_Text[i].setFont(m_arialFont.getFont());
			m_Text[i].setTextColor(0xffffff);
			m_Text[i].setBackColor(0x0000ff);
		}
		
		char neededChar = whatTheyHaveToSay[0];
		int x = 0, y = 0, z = 0;
		char tehMessage[3][50] = {" "};
		while(neededChar != 0)
		{
			if(neededChar == 10)
			{
				m_Text[y].setMessage(tehMessage[y]);
				if(y < 3)
					y++;
				x = 0;
			}
			else
			{
				tehMessage[y][x] = neededChar;
				x++;
			}
			z++;
			neededChar = whatTheyHaveToSay[z];
		}
		m_Text[y].setMessage(tehMessage[y]);
	}
	void activateDialogue()
	{
		//If it is drawing, stop it. If it's not, then start.
		drawText = true;
	}
	void deactivateDialogue()
	{
		drawText = false;
	}
	void drawUnique(SDL_Surface *a_screen)
	{
		if(drawText)
		{
			SRect rect;
			rect.x = 0; rect.y = 400; rect.w = 800; rect.h = 168;
			SDL_FillRect(a_screen, &rect, 0x0000ff);
			for(int i = 0; i < 3; i++)
			{
				if(m_Text[i].messageAvailable())
					m_Text[i].printMessage(a_screen, 20, 420+i*25);
			}
		}
	}
	void updateUnique(int a_timePassed, World *a_world)
	{
		//Search for a PLAYER in its grid, if they're colliding, check to see if spacebar is pressed. If so, activate its dialogue.
		Entity * emptyPointer;
		if((emptyPointer = a_world->getPlayer()))
		{
			int range = (int)(FRAME_SIZE*1.25);
			//Player in the grid. I now have access to the player's location, AND ACTIVATION sequence.
			if(this->withinRange(emptyPointer, range) && emptyPointer->getFlag(FLAG_ACTIVE) && drawText == false)
				activateDialogue();
			if(!(this->withinRange(emptyPointer, range)))
				deactivateDialogue();
		}
	}
};
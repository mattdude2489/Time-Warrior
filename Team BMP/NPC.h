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
	TTtext m_Text; //The text variable.
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
		m_eType = NPC; 
		whatTheyHaveToSay = "Hi."; 
		drawText = false; 
		m_Text.setBackColor(0x0000ff);
		m_Text.setTextColor(0xffffff);
		m_arialFont.changeSizeTo(20);
		m_Text.setFont(m_arialFont.getFont()); 
		m_Text.setMessage(whatTheyHaveToSay);
	}
	void initNPC(char * talk)
	{
		m_eType = NPC;
		whatTheyHaveToSay = talk;
		drawText = false;
		m_Text.setBackColor(0x0000ff);
		m_Text.setTextColor(0xffffff);
		m_arialFont.changeSizeTo(20);
		m_Text.setFont(m_arialFont.getFont());
		m_Text.setMessage(whatTheyHaveToSay);
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
			m_Text.printMessage(a_screen, 20, 420);
		}
	}
	void updateUnique(int a_timePassed, World *a_world)
	{
		//Search for a PLAYER in its grid, if they're colliding, check to see if spacebar is pressed. If so, activate its dialogue.
		Entity * emptyPointer;
		if(a_world->getGrid(m_location.x, m_location.y)->getPlayer(emptyPointer))
		{
			//Player in the grid. I now have access to the player's location, AND ACTIVATION sequence.
			if(this->collideSimple(emptyPointer) && emptyPointer->getActivation() && drawText == false)
				activateDialogue();
			if(!(this->collideSimple(emptyPointer)))
				deactivateDialogue();
		}
	}
};
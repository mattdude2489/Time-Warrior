//Hi there. This is the NPC class.
//Created by Matt Morrill, edited last by Matt Morrill (who actually uses these?)

#pragma once
#include "trueTextPrinter.h"
#include "Entity.h"

class NonPlayerChar : public Entity
{
private:
	char * whatTheyHaveToSay; //Probably nothing worthwhile.
	TTtext m_Text; //The text variable.
	bool drawText;
public:
	NonPlayerChar(SDL_Sprite * a_sprite)
		:Entity(0, 0, 0, 10, 0, 0, 0, 0, a_sprite){m_eType = NPC; whatTheyHaveToSay = "Hi."; drawText = false; m_Text.setMessage(whatTheyHaveToSay);}
	NonPlayerChar(char* talk, SDL_Sprite *a_sprite)
		:Entity(0, 0, 0, 10, 0, 0, 0, 0, a_sprite)
	{
		m_eType = NPC;
		whatTheyHaveToSay = talk;
		drawText = false;
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
			SDL_FillRect(a_screen, &SRect(0, 0, 40, 40), 0);
			m_Text.printMessage(a_screen, 0, 0);
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
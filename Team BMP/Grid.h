//The Grid class system: 16 grids per world, each containing a template vector of entities.
//This way the only thing we need to do is update the grids surrounding the player to update the entities that MATTER.
//Grids will be separate from the Tile-based World system.
//Made by: Matthew Morrill. Edited last by: Matt Morrill

#pragma once
#include "Entity.h"
#include "templateVector.h"
#include <time.h>
enum e_grid {NUM_GRIDS = 16, NUM_GRIDS_PER_ROW_COL = 4, NUM_SPRITE_SHEETS_TO_CHOOSE_FROM = 2};
enum e_world {WORLD_HUB, WORLD_ENGLAND};

class Minion;

class Grid
{
private:
	//Private variables...aka the templateVector of Entities. Although perhaps a Linked List would be better in this case.
	//For now, let's try template Vector.
	TemplateVector2<Entity*> m_mapOfEntities;
public:
	//The public functions. Constructors, sets, gets, and modifiers.
	Grid() {}
	Grid(Entity * newEntity) {m_mapOfEntities.add(newEntity);} //Starting off with an original entity.
	~Grid()
	{
		if(m_mapOfEntities.size() > 0)
		{
			for(int i = 0; i < m_mapOfEntities.size(); i++)
			{
				if(m_mapOfEntities.get(i)->getNewed())
					delete m_mapOfEntities.get(i);
			}
		}
		m_mapOfEntities.release();
	}
	//Sets and gets.
	//@param: The entity to add to the grid.
	void setEntity(Entity * newEntity) {m_mapOfEntities.add(newEntity);}
	//Will randomly assign monsters in there. WHEN WORLD CHANGES, THIS WILL OVERWRITE ANY MONSTERS.
	void setMonsters(int currentWorld, int currentGrid, int maxX, int maxY);
	//@param: The index of the returning Entity. NO ERROR DETECTION...SO DON'T BE WRONG.
	Entity * getEntity(int index) 
	{
		if(index < 0 || index > m_mapOfEntities.size())
			return m_mapOfEntities.get(0);
		else
			return m_mapOfEntities.get(index);
	}
	//Returns the Player Character...if there is one.
	//@param: the entity pointer that becomes the player.
	//@return: the boolean the tells if the player was successfully retrieved.
	bool getPlayer(Entity * &emptyEntityPointer)
	{
		for(int i = 0; i < m_mapOfEntities.size(); i++)
		{
			if(m_mapOfEntities.get(i)->getType() == PLAYER)
			{
				emptyEntityPointer = m_mapOfEntities.get(i);
				return true;
			}
		}
		return false;
	}
	//@Return: returns the size of the current vector of the Grid.
	int getNumberOfEntities() {return m_mapOfEntities.size();}
	void remove(int a_index) {m_mapOfEntities.remove(a_index);}
	//Modifiers or callers.
	void draw(SDL_Surface * a_screen)
	{
		for(int i = 0; i < m_mapOfEntities.size(); ++i)
			m_mapOfEntities.get(i)->draw(a_screen);
	}
	void sortOnYPosition()
	{
		//Selection sort, using the Y position. Dear god. Let's hope it doesn't slow it down too much.
		for(int i = 0; i < m_mapOfEntities.size(); ++i)
		{
			for(int k = i; k < m_mapOfEntities.size(); ++k)
			{
				if(m_mapOfEntities.get(i)->getLocation().y > m_mapOfEntities.get(k)->getLocation().y)
				{
					m_mapOfEntities.swap(i, k);
				}
			}
		}
	}
	
	void update(Uint32 timePassed, World * a_world) 
	{
		for(int i = 0; i < m_mapOfEntities.size(); i++) 
			m_mapOfEntities.get(i)->update(timePassed, a_world);
	}
};
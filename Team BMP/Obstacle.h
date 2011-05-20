//Obstacle class
//Author: John Parks

#pragma once

enum e_obstacleType {PORTAL, DUNGEON, TREE, DOOR, NUM_OBSTACLE_TYPES};

class Obstacle : public Entity
{
private:
	bool m_oType[NUM_OBSTACLE_TYPES];
public:
	Obstacle(SDL_Sprite *a_sprite):Entity(a_sprite)
	{
		a_sprite->setLoopToBegin(true);
		m_eType = OBSTACLE;
		for(int i = 0; i < NUM_OBSTACLE_TYPES; ++i)
			m_oType[i] = false;
		m_index = 0;
	}
	bool isObstacle(e_obstacleType a_type){return m_oType[a_type];}
	int getIndex(){return m_index;}
	void setIndex(int in){m_index = in;}
	void setObstacleType(e_obstacleType a_type)
	{
		for(int i = 0; i < NUM_OBSTACLE_TYPES; ++i)
			m_oType[i] = false;
		m_oType[a_type] = true;
	}
};
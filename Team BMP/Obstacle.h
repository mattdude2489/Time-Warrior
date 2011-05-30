//Obstacle class
//Author: John Parks


#pragma once

enum e_obstacleType {PORTAL, DUNGEON, TREE, DOOR, BUILDING, NUM_OBSTACLE_TYPES};
enum e_doorLocation	{DOOR_BOTTOM, DOOR_RIGHT};
enum e_buildingType	{BUILD_WOOD, BUILD_STONE};

class Obstacle : public Entity
{
private:
	bool m_oType[NUM_OBSTACLE_TYPES];
	int doorFace, buildingType;
	SRect door;
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
	void setBuildType(int a_type){buildingType = a_type;}
	void setDoorFace(int a_face){doorFace = a_face;}
	void setDoor()
	{
		if(doorFace == DOOR_RIGHT)
			door.setPosition(SPoint(this->getLocation().getX()+this->getSprite()->getWidth(),this->getLocation().getY()));
		else
			door.setPosition(SPoint(this->getLocation().getX(),this->getLocation().getY()+this->getSprite()->getHeight()));
		door.setHeight(FRAME_SIZE/2);
		door.setWidth(FRAME_SIZE/2);
	}
	bool colideWithDoor(Entity * a_player){return door.intersects(SRect(a_player->getLocation().getX(), a_player->getLocation().getY(), a_player->getSprite()->getWidth(), a_player->getSprite()->getHeight()));}

};
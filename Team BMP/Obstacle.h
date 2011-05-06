//Obstacle class
//Author: John Parks

class Obstacle : public Entity
{
private:
	bool m_portal, m_dungeon;
	int m_index;
public:
	Obstacle(SDL_Sprite *a_sprite):Entity(a_sprite)
	{
		a_sprite->setLoopToBegin(true);
		m_eType = OBSTACLE;
		m_portal = m_dungeon = false;
		m_index = 0;
	}
	bool isDoor(){return m_eType == DOOR;}
	bool isDungeon(){return m_dungeon;}
	bool isPortal(){return m_portal;}
	int getIndex(){return m_index;}
	void setIndex(int in){m_index = in;}
	void setDoor(){m_eType = DOOR;}
	void setDungeon(){m_dungeon = true;m_portal = false;}
	void setPortal(){m_portal = true;m_dungeon = false;}
	void setTree(){m_eType = TREE;}
};
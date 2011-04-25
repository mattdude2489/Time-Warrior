//Obstacle class
//Author: John Parks

class Obstacle : public Entity
{
protected:
	bool m_portal, m_dungeon;
public:
	Obstacle(SDL_Sprite *a_sprite):Entity(a_sprite)
	{
		a_sprite->setLoopToBegin(true);
		m_eType = OBSTACLE;
		m_portal = m_dungeon = false;
	}
	bool isPortal(){return m_portal;}
	bool isDungeon(){return m_dungeon;}
	void setPortal(){m_portal = true;m_dungeon = false;}
	void setDungeon(){m_dungeon = true;m_portal = false;}
};
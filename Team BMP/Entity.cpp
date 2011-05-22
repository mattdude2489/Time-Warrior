#include "Entity.h"
#include "World.h"
#include "Chip.h"
#include "Obstacle.h"

void Entity::update(int a_timePassed, World * a_world)
{
	//Update position.
	movePlayer(a_timePassed);
	//update stats
	m_timers[TIMER_REGEN] += a_timePassed;
	if(m_timers[TIMER_REGEN] >= TIME_SECOND_MS)
	{
		m_timers[TIMER_REGEN] = 0;
		regen(m_stats[ENERGY_REGEN]);
		if(m_eType == DUMMY)
			heal(m_stats[ENERGY_REGEN]);
	}
	//update timer
	m_timers[TIMER_GENERAL] += a_timePassed;
	switch(m_eType)
	{
	case CHIP:
	case OBSTACLE:
		break;
	default:
		if(m_timers[TIMER_GENERAL] >= TIME_INACTIVE && m_sprite->getFrame() == 1)
			m_sprite->stop();
	}
	//update health-bar & sprite
	m_hb.w = (Uint16)(((double)getStatNumber(HEALTH_CURRENT)/(double)getStatNumber(HEALTH_MAX))*(double)m_sprite->getWidth());
	m_hb.h = 5;
	m_sprite->update(a_timePassed);
	//check for world collision/tile collision
	if(m_flags[FLAG_DRAW])
	{
		switch(m_eType)
		{
			case OBSTACLE:
				break;
			default:
				if(a_world->getTile(m_location.x, m_location.y)->collide
					|| a_world->getTile(m_location.x+m_sprite->getWidth(), m_location.y)->collide
					|| a_world->getTile(m_location.x, m_location.y+m_sprite->getHeight())->collide
					|| a_world->getTile(m_location.x+m_sprite->getWidth(), m_location.y+m_sprite->getHeight())->collide)
				{
					switch(m_eType)
					{
					case CHIP:	setDrawOff();	break;
					default:	move(m_prevLoc.x - m_location.x, m_prevLoc.y - m_location.y);
						if(m_eType == MINION)
							this->setStopped();
					}
				}
				if(m_eType == PLAYER)
				{
					if(a_world->getTile(m_location.x, m_location.y)->door
						|| a_world->getTile(m_location.x+m_sprite->getWidth(), m_location.y)->door
						|| a_world->getTile(m_location.x, m_location.y+m_sprite->getHeight())->door
						|| a_world->getTile(m_location.x+m_sprite->getWidth(), m_location.y+m_sprite->getHeight())->door
						|| a_world->getTile(m_location.x, m_location.y)->stairs
						|| a_world->getTile(m_location.x+m_sprite->getWidth(), m_location.y)->stairs
						|| a_world->getTile(m_location.x, m_location.y+m_sprite->getHeight())->stairs
						|| a_world->getTile(m_location.x+m_sprite->getWidth(), m_location.y+m_sprite->getHeight())->stairs)
					{
						char temp[20];
						sprintf_s(temp, "Maps/Castle%i.txt", a_world->getCastleCount());
						a_world->setWorld(temp);
						a_world->incCastleCount();
						a_world->setCamera(this->m_camera);
					}
				}
				
		}
		switch(m_eType)
		{
		case PLAYER:
		case MINION:
		case BOSS:
			//Search through the entities in that particular grid. If there are any, check for collision with them.
			for(int i = 0; i < a_world->getGrid(m_location.x, m_location.y)->getNumberOfEntities(); i++)
			{
				if(collide(a_world->getEntity(i, m_location.x, m_location.y)))
				{
					if(epicCollide(a_world->getEntity(i, m_location.x, m_location.y)))
					{
						Entity * tmp = a_world->getEntity(i, m_location.x, m_location.y);
						if(tmp->getType() != OBSTACLE)
							move(m_prevLoc.x - m_location.x, m_prevLoc.y - m_location.y);
						else
						{
							if(tmp->isObstacle(PORTAL))
							{
								if(m_eType == PLAYER)
								{
									//if(a_world->getCurrentWorld() == WORLD_D1)
									//	this->setLocation(1248, 128);
									if(a_world->getBossCount() == 0){
										a_world->setWorld("Maps/MedEngMap.txt");
										a_world->setCamera(this->m_camera);
									}
								}
							}
							else if(tmp->isObstacle(DUNGEON))
							{
								if(m_eType == PLAYER)
								{
									if(!a_world->isDClosed(tmp->getIndex())){//if the dungeon is not closed open it
										int dungeon = rand()%3;//picks a random dungeon 

										this->setLastW();
										a_world->setDClosed(tmp->getIndex());
										switch(dungeon){
											case 0:
												a_world->setWorld("Maps/Dungeon1.txt");
												break;
											case 1:
												a_world->setWorld("Maps/Dungeon0.txt");
												break;
											case 2:
												a_world->setWorld("Maps/Dungeon2.txt");
												break;
										}
										a_world->setCamera(this->m_camera);
									}
								}
							}
							else
								move(m_prevLoc.x - m_location.x, m_prevLoc.y - m_location.y);
						}
					}
				}
			}
		}
	}
	//update previous location & anything unique
	if(!m_prevLoc.equals(m_location))
		m_prevLoc = m_location;

	updateUnique(a_timePassed, a_world);
	useEffects(a_timePassed);
}
int Entity::getTotalDamageDealt(int a_amount, e_chipType a_type)
{
	int stat = 0;
	switch(a_type)
	{
	case MAGIC:		stat = getStatNumber(INTELLECT);	break;
	case WEAPON:	stat = getStatNumber(STRENGTH);		break;
	}
	if(a_amount > 0)
		return (int)(a_amount * ((double)(a_amount + stat) / a_amount));
	else
		return 0;
}
int Entity::getTotalDamageTaken(int a_amount, e_chipSubType a_type)
{
	int stat = 0;
	switch(a_type)
	{
	case DIVINE://since there is no Divine resistance, must set appropriate dmg here
		switch(m_mtrl)
		{
		case MTRL_LIGHT:	stat = (int)(a_amount*.5);	break;	//with dmg algorithm, equates to 1/3 dmg
		case MTRL_DARK:		break;	//already init to 0			//with dmg algorithm, equates to 1/1 dmg
		default:			stat = a_amount;					//with dmg algorithm, equates to 1/2 dmg
		}
		break;
	case LIGHTNING:	stat = getStatNumber(RESISTANCE_LIGHTNING);	break;
	case FIRE:		stat = getStatNumber(RESISTANCE_FIRE);		break;
	case ICE:		stat = getStatNumber(RESISTANCE_ICE);		break;
	default:		stat = getStatNumber(DEFENSE);
	}
	if(a_amount > 0)
		return (int)(a_amount * ((double)a_amount / (a_amount + stat)));
	else
		return 0;
}
void Entity::reallocateResistancesAccordingToMaterial()
{
	//because it changes stats, this func should only be caused ONCE per Entity instance
	static bool called = false;
	if(!called && m_mtrl != MTRL_DEFAULT)
	{
		called = true;
		e_stats strongest, weakest;
		/*
		should only store known resistances (RESISTANCE_FIRE, RESISTANCE_ICE, RESISTANCE_LIGHTNING)
			abbreviated R_F, R_I, R_L
		culminates in 6 possible combos:
			strong:	R_F		R_F		R_I		R_I		R_L		R_L
			weak:	R_I		R_L		R_F		R_L		R_F		R_I
		*/
		switch(m_mtrl)
		{
		case MTRL_FIRE:
			strongest = RESISTANCE_FIRE;
			weakest = RESISTANCE_ICE;
			break;
		case MTRL_METAL:
			strongest = RESISTANCE_FIRE;
			weakest = RESISTANCE_LIGHTNING;
			break;
		case MTRL_WOOD:
		case MTRL_DARK:
			strongest = RESISTANCE_ICE;
			weakest = RESISTANCE_FIRE;
			break;
		case MTRL_WATER:
			strongest = RESISTANCE_ICE;
			weakest = RESISTANCE_LIGHTNING;
			break;
		case MTRL_EARTH:
			strongest = RESISTANCE_LIGHTNING;
			weakest = RESISTANCE_FIRE;
			break;
		case MTRL_AIR:
		case MTRL_LIGHT:
		case MTRL_RUBBER:
			strongest = RESISTANCE_LIGHTNING;
			weakest = RESISTANCE_ICE;
		}
		int amtMoved = (int)(m_stats[weakest] * .5);
		m_stats[weakest] -= amtMoved;
		m_stats[strongest] += amtMoved;
	}
}
void Entity::setCurrentLocToLast(World * a_world){this->setLocation(m_lastWLoc);}
void Entity::useEffects(int a_timePassed)
{
	if(m_effects[KNOCKBACK].active)
	{
		m_effects[KNOCKBACK].timer += a_timePassed;
		if(moveTo(&m_effects[KNOCKBACK].target, (int)(SPEED_MAGIC*a_timePassed))
		|| m_effects[KNOCKBACK].timer > m_effects[KNOCKBACK].timeLimit)
			m_effects[KNOCKBACK].active = false;
	}
	if(m_effects[FREEZE].active)
	{
		m_effects[FREEZE].timer += a_timePassed;
		while(m_effects[FREEZE].timer > m_effects[FREEZE].timeInterval && m_effects[FREEZE].active)
		{
			m_effects[FREEZE].intervalCount++;
			m_effects[FREEZE].timer -= m_effects[FREEZE].timeInterval;
			hit(m_effects[FREEZE].dmg, ICE);
			if(m_effects[FREEZE].intervalCount > m_effects[FREEZE].intervalLimit)
				m_effects[FREEZE].active = false;
		}
	}
}
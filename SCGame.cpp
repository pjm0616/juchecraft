// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#include "config.h"

#ifdef __WIN32__
#include <windows.h>
#endif

#include "smart_ptrs.h"
#include <string>
#include <list>
#include <map>
#include <vector>
#include <cassert>

#ifdef DEBUG
#include <cstdio>
#include <cstdlib>
#endif

#ifndef __WIN32__
#include <sys/time.h>
#endif

#include "defs.h"
#include "compat.h"
#include "luacpp/luacpp.h"
#include "SCTypes.h"
#include "SCException.h"
#include "SCCoordinate.h"
#include "SCObject.h"
#include "SCObjectList.h"
#include "SCObjectIdList.h"
#include "SCObjectPrototypes.h"
#include "SCPlayer.h"
#include "SCGame.h"
#include "ui/SCGameUI.h"

using namespace SC;


Game::Game()
	:m_obj_protos(this), 
	m_start_time(0.0), 
	m_is_game_ended(false)
{
	this->initPlayers();
}

Game::~Game()
{
	this->removeAllObjects();
}

void Game::loadGameData(const char *dir)
{
	std::string basedir(dir);
	this->m_obj_protos.load(std::string(basedir + "./object_data/objects.dat").c_str());
}

const ObjectSPtr_t &Game::addObject(const ObjectSPtr_t &obj)
{
	obj->init();
	return this->m_objects.addObject(obj);
}

ObjectSPtr_t Game::newObject(ObjectId_t objid)
{
	return this->addObject(this->m_obj_protos.newObjectById(objid));
}

int Game::removeObject(const ObjectSPtr_t &obj)
{
	obj->cleanup();
	return this->m_objects.removeObject(obj);
}

void Game::removeAllObjects()
{
	ObjectList &objs = this->getObjectList();
	for(ObjectList::const_iterator it = objs.begin(); it != objs.end(); ++it)
	{
		const ObjectSPtr_t &obj = *it;
		obj->cleanup();
	}
	objs.clear();
}


double Game::getElapsedTime() const
{
#ifdef __WIN32__
	DWORD milisec = timeGetTime();
	return (double)milisec / 1000;
#else
	timeval tv;
	gettimeofday(&tv, NULL);
	double time = (tv.tv_sec - this->getStartTime()) + ((double)tv.tv_usec / 1000000);
	return time;
#endif
}

void Game::processObjects()
{
	ObjectList &objs = this->getObjectList();
#if 0
	for(ObjectList::iterator it = objs.begin(); it != objs.end(); ++it)
	{
		it->get()->processFrame();
	}
#else
	objs.resetIteratorChecker();
	for(ObjectList::const_iterator it = objs.begin(); it != objs.end(); )
	{
		const ObjectSPtr_t &obj = *it;
		obj->processFrame();
		if(objs.isIteratorInvalidated())
		{
			// FIXME: this is very inefficient
			// start over
			it = objs.begin();
			objs.resetIteratorChecker();
		}
		else
		{
			++it;
		}
	}
#endif
}

void Game::run()
{
	this->setStartTime(this->getElapsedTime());
	
	this->test_tmp1(); // debug
	
	// precalculate some constants
	float fixed_deltat = 1.0 / this->getUpdateRate();
	unsigned long sleep_time = 1000000.0 / this->getUpdateRate();
	float fixed_frame_deltat = 1.0 / this->m_ui->getFPS();
	
	// initizlize loop variables
	this->setDelta(fixed_deltat);
	this->setFrameNumber(0);
	this->setFrameDelta(fixed_frame_deltat);
	this->setLastDrawTime(0.0);
	double start_time, end_time = this->getElapsedTime();
	while(!this->isGameEnded())
	{
		//double start_time = this->getElapsedTime();
		start_time = end_time;
		this->setLastTicks(start_time * 1000);
		
		this->processObjects();
		this->m_ui->processFrame();
		
		double process_finished_time = start_time; // this->getElapsedTime();
		if(process_finished_time - this->getLastDrawTime() > fixed_frame_deltat)
		{
			this->m_ui->draw();
			
			this->increaseFrameNumber();
			double draw_finished_time = start_time; // this->getElapsedTime();
			this->setFrameDelta(draw_finished_time - this->getLastDrawTime());
			this->setLastDrawTime(draw_finished_time);
		}
		
		// limit update rate
		usleep(sleep_time);
		// calculate time
		end_time = this->getElapsedTime();
		this->setDelta(end_time - start_time);
	}
}



void Game::test_tmp1()
{
	ObjectSPtr_t o;
	
	const PlayerVector &players = this->getPlayers();
	players[1]->setRace(RaceId::Juche);
	players[2]->setRace(RaceId::Terran);
	players[1]->increaseMinerals(50);
	players[2]->increaseMinerals(50);
	
	#if 1
	for(int i = 0; i < 10; i++)
	{
		o = this->newObject(ObjectId::Resource_MineralField);
		o->setPosition(10, 270 + i*44);
	}
	for(int i = 0; i < 10; i++)
	{
		o = this->newObject(ObjectId::Resource_MineralField);
		o->setPosition(10 + i*64, 270 + 44*3);
	}
	#endif
	
	o = this->newObject(ObjectId::Juche_RodongCorrectionalFacility);
	o->changeOwner(players[1]);
	o->setPosition(80, 80);
	
	o = this->newObject(ObjectId::Terran_CommandCenter);
	o->changeOwner(players[1]);
	o->setPosition(440, 180);
	
	o = this->newObject(ObjectId::Terran_CommandCenter);
	o->changeOwner(players[2]);
	o->setPosition(260, 250);
	
	o = this->newObject(ObjectId::Juche_AojiWorker);
	o->changeOwner(players[1]);
	o->setPosition(200, 100);
	o->setObjectMovingSpeedBonusM(1.0);
	o->move(Coordinate(210, 300));
	
	o = this->newObject(ObjectId::Zerg_Zergling);
	o->changeOwner(players[1]);
	o->setPosition(80, 50);
	o->setObjectMovingSpeedBonusM(2.0);
	o->setObjectAttackSpeedBonusM(8.0);
	o->setObjectDamageBonusM(2.0);
	o->setObjectArmorBonusA(3.0);
	o->move(Coordinate(410, 250));
	
	o = this->newObject(ObjectId::Juche_DaepodongLauncher);
	o->changeOwner(players[1]);
	o->setPosition(40, 40);
	o->setObjectMovingSpeedBonusM(4.0);
	o->setObjectAttackSpeedBonusM(4.0);
	o->setObjectDamageBonusM(2.0);
	o->setObjectArmorBonusA(3.0);
}





ObjectSPtr_t Game::findObjectByRect(ObjectList &matched_objs, int left, int top, int right, int bottom)
{
	ObjectList &objs = this->getObjectList();
	
	matched_objs.clear();
	for(ObjectList::const_iterator it = objs.begin(); it != objs.end(); ++it)
	{
		const ObjectSPtr_t &obj = *it;
		if(obj->insideRect(left, top, right, bottom))
		{
			//fprintf(stderr, "adding object %s\n", obj->getObjectName());
			matched_objs.addObject(obj);
		}
	}
	
	if(matched_objs.empty())
		return ObjectSPtr_t();
	else
		return *matched_objs.begin();
}
ObjectSPtr_t Game::findObjectByRect(ObjectList &matched_objs, const Coordinate &top_left, const Coordinate &bottom_right)
{
	Coordinate top_left2(top_left), bottom_right2(bottom_right);
	Coordinate::normalizeTopLeftCoordinate(top_left2, bottom_right2);
	
	return this->findObjectByRect(matched_objs, top_left2.getX(), top_left2.getY(), bottom_right2.getX(), bottom_right2.getY());
}




// Player related
//@{

void Game::initPlayers()
{
	// 0xRRGGBB
	static unsigned int player_colors[] = {
		0x000000, // Black, Neutral
		0xff0000, // Red
		0x0000ff, // Blue
		0x008080, // Teal
		0x800080, // Purple
		0xffa500, // Orange
		0xa52a2a, // Brown
		0xffffff, // White
		0xffff00, // Yellow
	};
	
	this->m_players.clear();
	for(int i = 0; i < Player::MAX_PLAYER + 1; i++)
	{
		PlayerSPtr_t player(new Player);
		
		player->setPlayerId(i);
		player->setPlayerColor(player_colors[i]);
		if(i == Player::NeutralPlayer)
			player->setRace(RaceId::Neutral);
		else
			player->setRace(RaceId::Terran);
		
		this->m_players.push_back(player);
	}
}

const PlayerSPtr_t &Game::getPlayer(PlayerId_t player_id) const
{
	if(unlikely(player_id < 0 || player_id >= (PlayerId_t)this->m_players.size()))
		throw new Exception("Invalid player id");
	
	return this->m_players[player_id];
}
//@}














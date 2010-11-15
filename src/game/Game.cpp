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
#include <deque>
#include <cassert>

#ifdef DEBUG
#include <cstdio>
#include <cstdlib>
#endif

#ifndef __WIN32__
#include <time.h>
#include <sys/time.h>
#endif

#include "defs.h"
#include "compat.h"
#include "luacpp/luacpp.h"
#include "SCTypes.h"
#include "SCException.h"
#include "SCCoordinate.h"
#include "SCObjectIdList.h"
#include "actions/UnitAction.h"
#include "orders/UnitOrder.h"
#include "SCObject.h"
#include "SCObjectList.h"
#include "SCObjectFactory.h"
#include "SCPlayer.h"
#include "SCGame.h"
#include "ui/SCGameUI.h"

using namespace SC;


Game::Game()
	: m_obj_factory(this)
	, m_start_time(0.0)
	, m_is_game_ended(false)
{
	this->initPlayers();
}

Game::~Game()
{
	// almost all objects will be freed here
	this->removeAllObjects();
	
	// delete ui before removing players, because ui is associated with player.
	this->m_ui = NULL; // now there's no game UI. UI functions must not be called from now on.
	// some objects(selected by player) will be freed here
	this->removeAllPlayers();
}

void Game::loadGameData(const char *dir)
{
	std::string basedir(dir);
	this->m_obj_factory.load(std::string(basedir + "./object_data/objects.dat").c_str());
}

const ObjectPtr &Game::addObject(const ObjectPtr &obj)
{
	obj->init();
	return this->m_objects.addObject(obj);
}

ObjectPtr Game::newObject(ObjectId_t objid)
{
	return this->addObject(this->m_obj_factory.newObjectById(objid));
}

int Game::removeObject(const ObjectPtr &obj)
{
	obj->cleanup();
	return this->m_objects.removeObject(obj);
}

void Game::removeAllObjects()
{
	for(ObjectList::const_iterator it = this->m_objects.begin(); 
		it != this->m_objects.end(); ++it)
	{
		const ObjectPtr &obj = *it;
		obj->cleanup();
	}
	this->m_objects.clear();
}

void Game::removeAllPlayers()
{
	for(PlayerVector::const_iterator it = this->m_players.begin(); 
		it != this->m_players.end(); ++it)
	{
		delete *it;
	}
	this->m_players.clear();
}

void Game::startTimer()
{
#ifdef __WIN32__
	::timeBeginPeriod(1);
#endif
	this->setStartTime(this->getElapsedTime());
}

void Game::endTimer()
{
#ifdef __WIN32__
	::timeEndPeriod(1);
#endif
}

double Game::getElapsedTime() const
{
#ifdef __WIN32__
	DWORD milisec = ::timeGetTime();
	return (double)milisec / 1000;
#else
	// it seems that gettimeofday is faster than clock_gettime..
# if 0
	struct timespec tsp;
	::clock_gettime(CLOCK_MONOTONIC, &tsp);
	double time = (tv.tv_sec - this->getStartTime()) + ((double)tv.tv_nsec / 1000000000);
	return time;
# else
	struct timeval tv;
	::gettimeofday(&tv, NULL);
	double time = (tv.tv_sec - this->getStartTime()) + ((double)tv.tv_usec / 1000000);
	return time;
# endif
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
		const ObjectPtr &obj = *it;
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
	this->startTimer();
	
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
	
	// main game loop
	while(!this->isGameEnded())
	{
		//double start_time = this->getElapsedTime();
		start_time = end_time;
		
		this->processObjects();
		this->m_ui->processFrame();
		
		double process_finished_time = start_time; // this->getElapsedTime();
		if(process_finished_time - this->getLastDrawTime() > fixed_frame_deltat)
		{
			this->m_ui->draw();
			
			this->increaseFrameNumber();
			#if 0
			double draw_finished_time = start_time;
			#else
			double draw_finished_time = this->getElapsedTime();
			#endif
			this->setFrameDelta(draw_finished_time - this->getLastDrawTime());
			this->setLastDrawTime(draw_finished_time);
		}
		
		// limit update rate
		::usleep(sleep_time);
		// calculate time
		end_time = this->getElapsedTime();
		this->setDelta(end_time - start_time);
		this->setCachedElapsedTime(end_time);
	}
	
	this->endTimer();
}



void Game::test_tmp1()
{
	ObjectPtr o;
	
	const PlayerVector &players = this->getPlayers();
	players[1]->setRace(RaceId::Juche);
	players[2]->setRace(RaceId::Terran);
	players[1]->increaseMinerals(50);
	players[2]->increaseMinerals(50);
	
	#if 1
	for(int i = 0; i < 10; i++)
	{
		o = this->newObject(ObjectId::Resource_MineralField);
		o->setPosition(50, 270 + i*44);
	}
	for(int i = 0; i < 10; i++)
	{
		o = this->newObject(ObjectId::Resource_MineralField);
		o->setPosition(50 + i*64, 270 + 44*3);
	}
	#endif
	
	o = this->newObject(ObjectId::Juche_RodongCorrectionalFacility);
	o->changeOwner(players[1]);
	o->setPosition(80, 80);
	
	o = this->newObject(ObjectId::Terran_CommandCenter);
	o->changeOwner(players[1]);
	o->setPosition(470, 200);
	
	o = this->newObject(ObjectId::Terran_CommandCenter);
	o->changeOwner(players[2]);
	o->setPosition(280, 270);
	
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





ObjectPtr Game::findObjectByRect(ObjectList &matched_objs, int left, int top, int right, int bottom)
{
	ObjectList &objs = this->getObjectList();
	
	matched_objs.clear();
	for(ObjectList::const_iterator it = objs.begin(); it != objs.end(); ++it)
	{
		const ObjectPtr &obj = *it;
		if(obj->insideRect(left, top, right, bottom))
		{
			//fprintf(stderr, "adding object %s\n", obj->getObjectName());
			matched_objs.addObject(obj);
		}
	}
	
	if(matched_objs.empty())
		return ObjectPtr();
	else
		return *matched_objs.begin();
}
ObjectPtr Game::findObjectByRect(ObjectList &matched_objs, const Coordinate &top_left, const Coordinate &bottom_right)
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
		Player *player = new Player(this);
		
		player->setPlayerId(i);
		player->setPlayerColor(player_colors[i]);
		if(i == Player::NeutralPlayer)
			player->setRace(RaceId::Neutral);
		else
			player->setRace(RaceId::Terran);
		
		this->m_players.push_back(player);
	}
}

Player *Game::getPlayer(PlayerId_t player_id) const
{
	if(unlikely(player_id < 0 || player_id >= (PlayerId_t)this->m_players.size()))
		throw new Exception("Invalid player id");
	
	return this->m_players[player_id];
}
//@}














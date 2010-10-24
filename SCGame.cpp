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
#include "SCException.h"
#include "SCCoordinate.h"
#include "SCPlayer.h"
#include "SCObject.h"
#include "SCObjectList.h"
#include "SCObjectIdList.h"
#include "SCObjectPrototypes.h"
#include "SCGame.h"
#include "ui/SCUserInterface.h"

using namespace SC;


Game::Game()
	:m_obj_protos(this), 
	m_start_time(0.0), 
	m_is_game_ended(false)
{
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
	return this->addObject(this->m_obj_protos.newObjectByObjectId(objid));
}

int Game::removeObject(const ObjectSPtr_t &obj)
{
	obj->cleanup();
	return this->m_objects.removeObject(obj);
}

void Game::removeAllObjects()
{
	ObjectSList &objs = this->getObjectList();
	for(ObjectSList::const_iterator it = objs.begin(); it != objs.end(); ++it)
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
	ObjectSList &objs = this->getObjectList();
#if 0
	for(ObjectList::iterator it = objs.begin(); it != objs.end(); ++it)
	{
		it->get()->processFrame();
	}
#else
	objs.resetIteratorChecker();
	for(ObjectSList::const_iterator it = objs.begin(); it != objs.end(); )
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
	while(!this->isGameEnded())
	{
		double start_time = this->getElapsedTime();
		
		this->processObjects();
		this->m_ui->processFrame();
		
		double process_finished_time = this->getElapsedTime();
		if(process_finished_time - this->getLastDrawTime() > fixed_frame_deltat)
		{
			this->m_ui->draw();
			
			this->increaseFrameNumber();
			double draw_finished_time = this->getElapsedTime();
			this->setFrameDelta(draw_finished_time - this->getLastDrawTime());
			this->setLastDrawTime(draw_finished_time);
		}
		
		// limit update rate
		usleep(sleep_time);
		// calculate time
		this->setDelta(this->getElapsedTime() - start_time);
	}
}



void Game::test_tmp1()
{
	ObjectSPtr_t o;
	
	Player::Players[1].setRace(RaceId::Juche);
	Player::Players[2].setRace(RaceId::Terran);
	Player::Players[1].increaseMinerals(50);
	Player::Players[2].increaseMinerals(50);
	
	o = this->newObject(ObjectId::Juche_DaepodongLauncher);
	o->changeOwner(&Player::Players[1]);
	o->setPosition(40, 40);
	o->setObjectMovingSpeedBonusM(4.0);
	o->setObjectAttackSpeedBonusM(4.0);
	o->setObjectDamageBonusM(2.0);
	o->setObjectArmorBonusA(3.0);
	
	o = this->newObject(ObjectId::Zerg_Zergling);
	o->changeOwner(&Player::Players[1]);
	o->setPosition(80, 50);
	o->setObjectMovingSpeedBonusM(2.0);
	o->setObjectAttackSpeedBonusM(8.0);
	o->setObjectDamageBonusM(2.0);
	o->setObjectArmorBonusA(3.0);
	o->move(Coordinate(410, 250));
	
	o = this->newObject(ObjectId::Juche_AojiWorker);
	o->changeOwner(&Player::Players[1]);
	o->setPosition(200, 100);
	o->setObjectMovingSpeedBonusM(1.0);
	o->move(Coordinate(210, 300));
	
	o = this->newObject(ObjectId::Terran_CommandCenter);
	o->changeOwner(&Player::Players[1]);
	o->setPosition(440, 180);
	
	o = this->newObject(ObjectId::Juche_RodongCorrectionalFacility);
	o->changeOwner(&Player::Players[1]);
	o->setPosition(80, 80);
	
	#if 1
	for(int i = 0; i < 10; i++)
	{
		o = this->newObject(ObjectId::Resource_MineralField);
		o->setPosition(10, 270 + i*32);
	}
	for(int i = 0; i < 10; i++)
	{
		o = this->newObject(ObjectId::Resource_MineralField);
		o->setPosition(10 + i*32, 270 + 32*3);
	}
	#endif
}
























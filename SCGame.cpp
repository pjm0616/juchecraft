// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#include "config.h"

#ifdef DEBUG
#include <stdio.h>
#include <stdlib.h>
#endif

#ifdef __WIN32__
#include <windows.h>
#endif

#include "smart_ptrs.h"
#include <string>
#include <list>
#include <map>

#ifndef __WIN32__
#include <sys/time.h>
#endif

#include "defs.h"
#include "compat.h"
#include "SCException.h"
#include "SCCoordinate.h"
#include "SCPlayer.h"
#include "SCObject.h"
#include "SCObjectList.h"
#include "SCGame.h"
#include "objects/SCObjects.h"
#include "ui/SCUserInterface.h"

using namespace SC;


Game::Game()
	:m_start_time(0), 
	m_frame_delta(0.0), 
	m_elapsed_time(0.0), 
	m_last_draw(0.0), 
	m_is_game_ended(false)
{
}

Game::~Game()
{
}

double Game::getTime() const
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
		Object *obj = it->get();
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
	this->setStartTime(this->getTime());
	
	this->test_tmp1(); // debug
	
	this->setFrameNumber(0);
	this->setFrameDelta(1.0 / this->getFPS());
	while(!this->isGameEnded())
	{
		double frame_start_time = getTime();
		
		this->processObjects();
		this->m_ui->processFrame();
		if(this->getElapsedTime() - this->getLastDrawTime() > (1.0 / this->m_ui->getUIFPS()))
		{
			this->m_ui->draw();
			this->setLastDrawTime(this->getElapsedTime());
		}
		
		// limit fps
		usleep(1000000.0 / this->getFPS()); // avg. 30fps
		// calculate time
		this->increaseFrameNumber();
		this->setFrameDelta(getTime() - frame_start_time);
		this->increaseElapsedTime(this->getFrameDelta());
	}
}


void Game::test_tmp1()
{
	Object *o;
	
	Player::Players[1].setRace(RaceId::Juche);
	Player::Players[2].setRace(RaceId::Terran);
	Player::Players[1].increaseMinerals(50);
	Player::Players[2].increaseMinerals(50);
	
	
	o = this->addObject(new Objects::Units::Terran_Marine(this));
	o->changeOwner(&Player::Players[2]);
	o->setPosition(40, 40);
	o->setObjectMovingSpeedBonusM(4.0);
	o->setObjectAttackSpeedBonusM(4.0);
	o->setObjectDamageBonusM(2.0);
	o->setObjectArmorBonusA(3.0);
	
	o = this->addObject(new Objects::Units::Zerg_Zergling(this));
	o->changeOwner(&Player::Players[2]);
	o->setPosition(80, 50);
	o->setObjectMovingSpeedBonusM(2.0);
	o->setObjectAttackSpeedBonusM(8.0);
	o->setObjectDamageBonusM(2.0);
	o->setObjectArmorBonusA(3.0);
	o->move(Coordinate(410, 250));
	
	o = this->addObject(new Objects::Units::Juche_AojiWorker(this));
	o->changeOwner(&Player::Players[1]);
	o->setPosition(200, 100);
	o->setObjectMovingSpeedBonusM(1.0);
	o->move(Coordinate(210, 300));
	
	o = this->addObject(new Objects::Buildings::Terran_CommandCenter(this));
	o->changeOwner(&Player::Players[2]);
	o->setPosition(440, 180);
	
	o = this->addObject(new Objects::Buildings::Juche_RodongCorrectionalFacility(this));
	o->changeOwner(&Player::Players[1]);
	o->setPosition(80, 80);
	
	#if 1
	for(int i = 0; i < 10; i++)
	{
		o = this->addObject(new Objects::Resources::MineralField(this));
		o->setPosition(10, 270 + i*32);
	}
	for(int i = 0; i < 10; i++)
	{
		o = this->addObject(new Objects::Resources::MineralField(this));
		o->setPosition(10 + i*32, 270 + 32*3);
	}
	#endif
}
























// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details


#ifdef DEBUG
#include <stdio.h>
#include <stdlib.h>
#endif

#include <tr1/memory>
#include <string>
#include <list>
#include <map>

#include <sys/time.h>

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
	:objects(*this), 
	start_time(0), frame_delta(0.0), elapsed_time(0.0), last_draw(0.0)
{
}

Game::~Game()
{
}

double Game::getTime() const
{
	timeval tv;
	gettimeofday(&tv, NULL);
	double time = (tv.tv_sec - this->start_time) + ((double)tv.tv_usec / 1000000);
	return time;
}

void Game::processObjects()
{
#if 0
	for(ObjectList::iterator it = this->objects.begin(); it != this->objects.end(); ++it)
	{
		it->get()->processFrame();
	}
#else
	this->objects.resetIteratorChecker();
	for(ObjectList::const_iterator it = this->objects.begin(); 
		it != this->objects.end(); )
	{
		Object *obj = it->get();
		obj->processFrame();
		if(this->objects.isIteratorInvalidated())
		{
			// FIXME: this is very inefficient
			// start over
			it = this->objects.begin();
			this->objects.resetIteratorChecker();
		}
		else
			++it;
	}
#endif
}

void Game::run()
{
	this->start_time = this->getTime();
	
	//system("reset");
	this->test_tmp1(); // debug
	
	this->frame_number = 0;
	this->frame_delta = 1.0 / this->getFPS();
	while(1)
	{
		double frame_start_time = getTime();
		
		this->processObjects();
		this->ui->processFrame();
		if(this->elapsed_time - this->last_draw > (1.0/this->ui->getUIFPS()))
		{
			this->ui->draw();
			this->last_draw = this->elapsed_time;
		}
		
		// limit fps
		usleep((double)1000000 / this->getFPS()); // avg. 30fps
		// calculate time
		this->frame_number++;
		this->frame_delta = getTime() - frame_start_time;
		this->elapsed_time += this->frame_delta;
	}
}


void Game::test_tmp2()
{
	
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
	o->setObjectAttackSpeedBonusM(8.0);
	o->setObjectDamageBonusM(2.0);
	
	o = this->addObject(new Objects::Units::Zerg_Zergling(this));
	o->changeOwner(&Player::Players[2]);
	o->setPosition(80, 50);
	o->setObjectMovingSpeedBonusM(2.0);
	o->setObjectAttackSpeedBonusM(8.0);
	o->setObjectDamageBonusM(2.0);
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
























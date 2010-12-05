/*
 *	Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
 *	This file is a part of JucheCraft. http://sigkill.kr/projects/juchecraft
 *	
 *	See LICENSE.txt for details.
**/

/** @file src/main.cpp
**/

#include "config.h"

#include <cstdio>

#include "smart_ptrs.h"
#include <iostream>
#include <string>
#include <list>
#include <map>
#include <vector>
#include <deque>
#include <clocale>

#include <cstring>
#include <cstdlib>

#include <time.h>

#ifdef __WIN32__
# include <windows.h>
#endif

#ifdef _OPENMP
# include <omp.h>
#endif

#include "defs.h"
#include "compat.h"
#include "luacpp/luacpp.h"
#include "game/Types.h"
#include "game/Exception.h"
#include "game/Coordinate.h"
#include "game/Target.h"
#include "game/ObjectIdList.h"
#include "game/actions/Actions.h"
#include "game/orders/Orders.h"
#include "game/UnitProductionManager.h"
#include "game/Object.h"
#include "game/ObjectList.h"
#include "game/ObjectFactory.h"
#include "game/Player.h"
#include "game/Game.h"

#include "ui/GameUI.h"
#include "ui/dummy/GameUI_dummy.h"
#include "ui/ncurses/GameUI_ncurses.h"
#include "ui/sdl/GameUI_SDL.h"



int main(int argc, char *argv[])
{
	//std::setlocale(LC_ALL, "");
	std::setlocale(LC_ALL, "en_US.utf8");
	
	#ifdef _OPENMP
	omp_set_num_threads(omp_get_max_threads());
	#endif
	
	srand48(time(NULL) ^ getpid());
	
	try
	{
		/* TODO: SC::GameUI is dedicated to Game UI. 
		 * In order to implement game main screen, menus, game lobby, etc we need to change current design.
		 * Create a base class for all UIs, and implement game main screen class etc.
		 */
		
		SC::Game game;
		game.setMapSize(640*2, 480*2);
		game.loadGameData(GAME_ROOT_DIR "./res/game/");
		
		SC::Player *my_player = game.getPlayer(1);
		
		fprintf(stderr, "Initializing UI...\n");
		SC::GameUI *game_ui;
		if(argc >= 2 && !strcmp(argv[1], "dummy"))
		{
			game_ui = new SC::GameUI_dummy(&game, my_player);
		}
		else
		#ifndef NO_NCURSES_UI
		if(argc >= 2 && !strcmp(argv[1], "ncurses"))
		{
			game_ui = new SC::GameUI_ncurses(&game, my_player);
			static_cast<SC::GameUI_ncurses *>(game_ui)->load_resources(GAME_ROOT_DIR "./res/ui/ncurses/objects/");
		}
		else
		#endif
		{
			game_ui = new SC::GameUI_SDL(&game, my_player);
		}
		
		fprintf(stderr, "Creating UI...\n");
		game_ui->initUI();
		
		game.setUI(game_ui);
		//fprintf(stderr, "Starting game...\n");
		game.run();
		
		game_ui->cleanupUI();
		
		delete game_ui;
	}
	catch(SC::Exception *e)
	{
		std::cerr << "Error: " << e->getErrorMsg() << std::endl;
		abort();
	}
	
	return 0;
}

#ifdef __WIN32__
int WinMain(HINSTANCE,HINSTANCE,LPSTR,int)
{
	char *argv[] = {"mini_sc", NULL}; // fixme
	return main(1, argv);
}
#endif




// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#include "config.h"

#include <cstdio>

#include "smart_ptrs.h"
#include <iostream>
#include <string>
#include <list>
#include <map>
#include <vector>
#include <clocale>

#include <cstring>
#include <cstdlib>

#include <time.h>

#ifdef __WIN32__
# include <windows.h>
#endif

#include "defs.h"
#include "compat.h"
#include "luacpp/luacpp.h"
#include "SCTypes.h"
#include "SCException.h"
#include "SCCoordinate.h"
#include "SCObject.h"
#include "SCObjectList.h"
#include "SCObjectPrototypes.h"
#include "SCPlayer.h"
#include "SCGame.h"

#include "ui/SCGameUI.h"
#include "ui/ncurses/SCGameUI_ncurses.h"
#include "ui/sdl/SCGameUI_SDL.h"



int main(int argc, char *argv[])
{
	//std::setlocale(LC_ALL, "");
	std::setlocale(LC_ALL, "en_US.utf8");
	
	srand48(time(NULL) ^ getpid());
	
	try
	{
		// TODO: 지금의 SC::UserInterface 는 게임 UI 전용.
		// 따라서 게임 시작 전 메뉴/맵 선택 등의 화면을 만들 수 없음. 그러니까 
		// SC::GameUserInterface 로 분리하던지
		// SC::UserInterface 를 개편하던지
		// 택 1
		
		SC::Game game;
		game.setMapSize(640*2, 480*2);
		game.loadGameData(GAME_ROOT_DIR "./res/game/");
		
		const SC::PlayerSPtr_t &my_player = game.getPlayer(1);
		
		fprintf(stderr, "Initializing UI...\n");
		SC::GameUI *game_ui;
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




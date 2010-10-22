// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#include "config.h"

#include <cstdio>

#include "smart_ptrs.h"
#include <iostream>
#include <string>
#include <list>
#include <map>
#include <clocale>

#include <cstring>
#include <cstdlib>

#include <time.h>

#ifdef __WIN32__
# include <windows.h>
#endif

#include "defs.h"
#include "compat.h"
#include "SCException.h"
#include "SCCoordinate.h"
#include "SCPlayer.h"
#include "SCObject.h"
#include "SCObjectList.h"
#include "SCGame.h"

#include "ui/SCUserInterface.h"
#include "ui/SCUserInterface_ncurses.h"
#include "ui/SCUserInterface_SDL.h"



int main(int argc, char *argv[])
{
	//std::setlocale(LC_ALL, "");
	std::setlocale(LC_ALL, "en_US.utf8");
	
	srand48(time(NULL) ^ getpid());
	
	SC::Player::initialize();
	#ifndef NO_NCURSES_UI
	SC::UserInterface_ncurses::load_resources(GAME_ROOT_DIR "./res/ui/ncurses/objects/");
	#endif
	
	try
	{
		// TODO: 지금의 SC::UserInterface 는 게임 UI 전용.
		// 따라서 게임 시작 전 메뉴/맵 선택 등의 화면을 만들 수 없음. 그러니까 
		// SC::GameUserInterface 로 분리하던지
		// SC::UserInterface 를 개편하던지
		// 택 1
		
		SC::Game game;
		game.setMapSize(640*2, 480*2);
		
		fprintf(stderr, "Initializing UI...\n");
		SC::UserInterface *ui;
		#ifndef NO_NCURSES_UI
		if(argc >= 2 && !strcmp(argv[1], "ncurses"))
			ui = new SC::UserInterface_ncurses(&game);
		else
		#endif
			ui = new SC::UserInterface_SDL(&game);
		
		fprintf(stderr, "Creating UI...\n");
		ui->initUI();
		
		game.setUI(ui);
		//fprintf(stderr, "Starting game...\n");
		game.run();
		
		ui->cleanupUI();
		
		delete ui;
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




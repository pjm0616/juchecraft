// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#include "SCPlayer.h"

using namespace SC;


bool Player::ms_is_initialized = false;
Player Player::Players[Player::MAX_PLAYER + 1];

void Player::initialize()
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
	
	for(int i = 0; i < Player::MAX_PLAYER + 1; i++)
	{
		Player &player = Player::Players[i];
		
		player.setPlayerId(i);
		player.setPlayerColor(player_colors[i]);
		
		if(i == Player::NeutralPlayer)
			player.setRace(RaceId::Neutral);
		else
			player.setRace(RaceId::Terran);
	}
	Player::ms_is_initialized = true;
}





Player::Player()
{
	this->setMinerals(0);
	this->setVespeneGas(0);
	for(int i = 0; i < RaceId::Size; i++)
	{
		this->setCurrentSupplies(i, 0);
		this->setSuppliesInUse(i, 0);
	}
	
	this->setPlayerArmorBonusA(0.0);
	this->setPlayerDamageBonusA(0.0);
}
















/*
 * RPS_FightInfo.cpp
 *
 *  Created on: May 3, 2018
 *      Author: tomhe
 */

#include "RPS_FightInfo.h"

char RPS_FightInfo::getPiece(int player)const {
	if (player==1) {
		return player1_piece;
	}
	else if (player==2) {
		return player2_piece;
	}
	return 0; //illegal player
}

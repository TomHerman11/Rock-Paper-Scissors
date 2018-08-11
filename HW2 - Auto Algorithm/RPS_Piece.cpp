/*
 * RPS_Piece.cpp
 *
 *  Created on: Apr 29, 2018
 *      Author: tomhe
 */

#include "RPS_Piece.h"

int Piece::isMovingPiece()const {
	if (type=='R' || type=='P' || type=='S') { //joker is a moving piece also, and can represent bomb.
		return 1;
	}
	return 0;
}

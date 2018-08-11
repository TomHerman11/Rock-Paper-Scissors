/*
 * Piece.cpp
 *
 *  Created on: 1 ???? 2018
 *      Author: Vered
 */

#include "Piece.h"

int Piece::isMovingPiece()const {
	if (type=='R' || type=='P' || type=='S') { //joker is a moving piece also, and can represent bomb.
		return 1;
	}
	return 0;
}

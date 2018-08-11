/*
 * RPS_PiecePosition.h
 *
 *  Created on: Apr 29, 2018
 *      Author: tomhe
 */

#ifndef RPS_PIECEPOSITION_H_
#define RPS_PIECEPOSITION_H_


#include "PiecePosition.h"
#include "RPS_Point.h"


class RPS_PiecePosition: public PiecePosition {
	char type;
	char jokerRep;
	RPS_Point point;

public:
	//RPS_PiecePosition(): type(0), jokerRep(0), point(0,0) {}
	RPS_PiecePosition(char type, char jokerRep, int x, int y): type(type), jokerRep(jokerRep), point(x,y) {}

	virtual const Point& getPosition()const override {return point;}
	virtual char getPiece()const override {return type;} // R, P, S, B, J or F
	virtual char getJokerRep()const override {return jokerRep;} // ONLY for Joker: R, P, S or B -- non-Joker may return ‘#’
	//~RPS_PiecePosition() {}

};

#endif /* RPS_PIECEPOSITION_H_ */

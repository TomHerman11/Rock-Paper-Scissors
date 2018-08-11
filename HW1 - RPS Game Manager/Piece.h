/*
 * Piece.h
 *
 *  Created on: Apr 3, 2018
 *      Author: tomhe
 */

#ifndef PIECE_H_
#define PIECE_H_

class Piece{
	char type;
	int isJoker;
	int ownerPlayer;


public:
	Piece(): type(0), isJoker(0), ownerPlayer(0){}
	Piece(char type, int isjoker, int ownerplayer): type(type), isJoker(isjoker), ownerPlayer(ownerplayer){}

	char getType()const { return type;}
	int getIsJoker()const {return isJoker;}
	int getOwnerPlayer()const {return ownerPlayer;}
	void setType(char newType) {type=newType;}
	int isMovingPiece()const;

};

#endif /* PIECE_H_ */

/*
 * RPS_FightInfo.h
 *
 *  Created on: Apr 29, 2018
 *      Author: tomhe
 */

#ifndef RPS_FIGHTINFO_H_
#define RPS_FIGHTINFO_H_

#include "RPS_Point.h"
#include "FightInfo.h"

class RPS_FightInfo: public FightInfo {
	const RPS_Point point;
	char player1_piece;
	char player2_piece;
	int winner;

public:
	RPS_FightInfo(): point(0,0), player1_piece(0), player2_piece(0), winner(0) {}
	RPS_FightInfo(int col, int row, char player1_piece, char player2_piece, int winner):
			point(col, row), player1_piece(player1_piece), player2_piece(player2_piece), winner(winner) {}

	/*
	void setPoint(const Point& _point) {this->point=RPS_Point(_point.getX(), _point.getY());}
	void setPlayer1Piece(char _player1_piece) {this->player1_piece=_player1_piece;}
	void setPlayer2Piece(char _player2_piece) {this->player2_piece=_player2_piece;}
	void setWinner(int _winner) {this->winner=_winner;}
	*/

	virtual const Point& getPosition()const override {return point;}
	virtual char getPiece(int player)const override; // R, P, S, B or F (but NOT J)
	virtual int getWinner()const override {return winner;} // 0 - both lost / tie, 1 - player 1 won, 2 - player 2 won
	virtual ~RPS_FightInfo() {}



};

#endif /* RPS_FIGHTINFO_H_ */

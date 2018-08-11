/*
 * RPS_Board.h
 *
 *  Created on: Apr 29, 2018
 *      Author: tomhe
 */

#ifndef RPS_BOARD_H_
#define RPS_BOARD_H_

#include "Board.h"
#include "RPS_Piece.h"
#include "Point.h"
#include <iostream>

#define M_COLUMNS 10
#define N_ROWS 10
#define R_MAX 2
#define P_MAX 5
#define S_MAX 1
#define B_MAX 2
#define J_MAX 2
#define F_MAX 1

class RPS_Board: public Board {
	Piece board[N_ROWS][M_COLUMNS];


public:
	virtual int getPlayer(const Point& pos)const override; // 1 for player 1’s piece, 2 for 2, 0 if empty
	//virtual ~RPS_Board() {}
	Piece& getPiece(const Point& point);
	void setPieceInPos(Piece& piece, const Point& point);
	void setEmptyPieceInPos(const Point& point);

	void printBoard();

};






#endif /* RPS_BOARD_H_ */

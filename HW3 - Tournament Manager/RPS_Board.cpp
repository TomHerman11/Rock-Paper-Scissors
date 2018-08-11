/*
 * RPS_Board.cpp
 *
 *  Created on: Apr 29, 2018
 *      Author: tomhe
 */

#include "RPS_Board.h"

void RPS_Board::printBoard() {
	//string reason;
	Piece piece;
	char pieceType;

	//now print the board
	for (int i=0; i<N_ROWS; i++) {
		for (int j=0; j<M_COLUMNS; j++) {
			piece= board[i][j];
			pieceType=piece.getType();
			if (piece.getOwnerPlayer()==0) { //"null square"
				std::cout << "_";
			}
			//o/w it belongs to some player!
			else if (piece.getOwnerPlayer()==1) {
				if (pieceType==0) {std::cout << "#";}
			/*	if (piece.getIsJoker()) {
					cout << "J";
				}*/
			//	else {
					std::cout << pieceType;
				//}
			}
			else if (piece.getOwnerPlayer()==2) {
				//if (piece.getIsJoker()) {
					//cout << "j";
					//continue;
			//	}
				if (pieceType=='R') {std::cout << "r";}
				else if (pieceType=='P') {std::cout << "p";}
				else if (pieceType=='S') {std::cout << "s";}
				else if (pieceType=='B') {std::cout << "b";}
				else if (pieceType=='F') {std::cout << "f";}
				else {	std::cout << "#";}
			}
		}
		std::cout << std::endl;
	}
}




int RPS_Board::getPlayer(const Point& pos)const {
	const int col=pos.getX();
	const int row=pos.getY();
	return board[row-1][col-1].getOwnerPlayer();
}

Piece& RPS_Board::getPiece(const Point& point) {
	int row= point.getY()-1;
	int col= point.getX()-1;
	return board[row][col];
}

void RPS_Board::setPieceInPos(Piece& piece, const Point& point){
		int row= point.getY()-1;
		int col= point.getX()-1;
		board[row][col]= piece;
}

void RPS_Board::setEmptyPieceInPos(const Point& point){
	int row= point.getY()-1;
	int col= point.getX()-1;
	board[row][col]= Piece();
}

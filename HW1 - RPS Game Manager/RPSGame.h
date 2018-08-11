/*
 * RPSGame.h
 *
 *  Created on: 1 ???? 2018
 *      Author: Vered
 */

#ifndef RPSGAME_H_
#define RPSGAME_H_

#include "Piece.h"
#include "LosingDetails.h"
#include "RPSParserMove.h"
#include "RPSParserPosition.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <cmath>
using namespace std;

#define M_COLUMNS 10
#define N_ROWS 10
#define R_MAX 2
#define P_MAX 5
#define S_MAX 1
#define B_MAX 2
#define J_MAX 2
#define F_MAX 1

class RPSGame{
	Piece board[N_ROWS][M_COLUMNS];
	int movingPiecesPlayer1;
	int movingPiecesPlayer2;
	int flagsPlayer1;
	int flagsPlayer2;


public:
	RPSGame(): movingPiecesPlayer1(0), movingPiecesPlayer2(0), flagsPlayer1(0), flagsPlayer2(0) {}

	Piece& getPieceRPSGame(int x, int y);
	void setPieceRPSGame(const Piece &piece, int x, int y);

	/*
	 * move piece to the coordinates at <row_cor, column_coor>
	 * assumes that at <row_cor, column_coor> there isn't a piece of the same player!
	 */
	void movePiece(Piece &piece, int column_coor, int row_coor);

	/*
	 * update the fields of the game, once a move have been made on the piece.
	 * reduce the movingPiece field if necessary, same for flags.
	 */
	void reduceGameFieldsUponMoving(const Piece &piece);

	/*
	 * increase values of flags and moving pieces, called upon insertion ONLY!
	 */
	void increaseGameFieldsUponMoving(const Piece &tmpPiece);

	/*
	 * this function assumes that the input file at "path" is valid - by already opening and reading it!
	 * inserting the matching Pieces into the game.
	 * if needed, a fight between pieces will occur.
	 *
	 * @returns
	 * if the open of read functions fail (becuase of system fail, since we already checked validity of the input file),
	 * we will print an error and this player loses.
	 */
	int insertPlayerPieces(const string& path, int player);

	/*
	 * initialize the game by the input files.
	 * if an error occurred in opening one of the input files - a matching message is printed, and returns -1. o/w:
	 * returns
	 * 0 - if tie
	 * 1 - if player 1 wins
	 * 2 - if player 2 wins
	 * 3 - continue play
	 */
	int initRPSGame(LosingDetails &losingdetails, const string& path1, const string& path2);
	/*
	 * checks if there's a winner, and updates the losingdetails accordingly. returns 1 if there's a winner. 0 o/w.
	 */
	int checkWinnerAfterPosition(LosingDetails &losingdetails);

	/*
	 * executes the game. at each turn reads a line from one of the move files, player 1 starts. if one of the move files is done,
	 * the function continues to read until the other move file is done.
	 * the function updates the LosingDetails upon losing of one of the players, or in case of any TIE.
	 * returns 0 if an error occurred in opening one of the files or reading a line.
	 * returns 1 at success.
	 */
	int executeGame(const string& path1,const string& path2, LosingDetails &result);

	/*
	 * checks if the moves file's line is legal
	 * if not- prints information describing the problem, and return 0.
	 * if the move in the line is legal- return 1.
	 */
	int isValidMoveLine(int player, RPSParserMove &parserdLine);

	/*
	 * change representation for joker in the given position
	 * updates the player's movingPieces if needed
	 * Assumptions: position is legal and holds a joker
	 */
	void changeJokerRep(int row, int col, char rep);

	/*
	 * performs the turn according to the parseLine from RPSParserMove
	 */
	void executeMove(RPSParserMove &parsedLine, int player);

	/*
	 * returns if player has moving pieces
	 * if player!= 1 or 2, return -1
	 */
	int playerHasMovingPieces(int player);

	/*
	 * return if the player has any remaining flags
	 * if player!= 1 or 2, return -1
	 */
	int playerHasFlag(int player);

	/*
	 * for internal use only.
	 */
	void printBoard();
};

/*
 * return 1 if both MOVES files exist (and can be opened). otherwise- returns 0.
 */
int movesFilesExist(const string& path1,const string& path2);

/*
 * this function makes a fight between two pieces, and returns the winning piece.
 * assumes that the rival piece do not belong to the same player.
 */
int fightPieces(const Piece &piece1, const Piece &piece2);

/*
 * prints to the output file as was ordered in the instructions.
 */
void writeToOutputFile(const string &output_path, RPSGame &game, LosingDetails &losingdetails);

#endif /* RPSGAME_H_ */

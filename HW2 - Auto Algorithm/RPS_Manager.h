/*
 * RPS_Manager.h
 *
 *  Created on: Apr 29, 2018
 *      Author: tomhe
 */

#ifndef RPS_MANAGER_H_
#define RPS_MANAGER_H_

#include <vector>
#include <memory> // for unique_ptr
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "PlayerAlgorithm.h"
#include "RPS_Board.h"
#include "RPS_FightInfo.h"

using namespace std;

#define M_COLUMNS 10
#define N_ROWS 10
#define R_MAX 2
#define P_MAX 5
#define S_MAX 1
#define B_MAX 2
#define J_MAX 2
#define F_MAX 1

#define PLAYER_BAD_POSITION 100
#define TIE_BAD_POSITION 101
#define PLAYER_FLAGS_EATEN 200
#define TIE_FLAGS_EATEN 201
#define PLAYER_NO_MOVING_PIECES 102
#define TIE_NO_MOVING_PIECES 202
#define ILLEGAL_MOVE 300
#define TOO_MANY_MOVES_WITH_NO_FIGHT 400

#define MAX_MOVES_WITHOUT_FIGHT 100

#define OUTPUT_FILE "rps.output"

class Manager {
	RPS_Board board;
	int movingPiecesPlayer1;
	int movingPiecesPlayer2;
	int flagsPlayer1;
	int flagsPlayer2;

public:
	Manager():board(), movingPiecesPlayer1(0),movingPiecesPlayer2(0), flagsPlayer1(0), flagsPlayer2(0) {}
	void writetoOutputFile(int winning_cause, int winner);
	int writetoOutputFileBadPosition(int legal_pos1, int legal_pos2);
	void increaseGameFieldsUponMoving(const Piece &tmpPiece);
	void insertPositionPieces(int player, vector<unique_ptr<PiecePosition>>& vectorPositions, vector<unique_ptr<FightInfo>>& vectorFights);
	void reduceGameFieldsUponMoving(const Piece &piece);
	//RPS_FightInfo Manager::movePiece(Piece &piece, const Point &point);
	void movePiece(Piece &piece, const Point &point, unique_ptr<FightInfo>& unq_ptr);
	int checkWinnerAfterPosition();
	void changeJokerRep(const JokerChange& jokerChange);
	int checkLegalMove(const Move& move, int player);
	int checkLegalJokerChange(const JokerChange& jokerChange, int player);
	int playerHasMovingPieces(int player);
	int playerHasFlag(int player);
	int checkWinner(int player );
	int game_manager(PlayerAlgorithm& player1, PlayerAlgorithm& player2);

};



#endif /* RPS_MANAGER_H_ */

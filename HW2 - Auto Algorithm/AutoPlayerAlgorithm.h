/*
 * AutoPlayerAlgorithm.h
 *
 *  Created on: May 7, 2018
 *      Author: tomhe
 */

#ifndef AUTOPLAYERALGORITHM_H_
#define AUTOPLAYERALGORITHM_H_

#include <stdio.h>
#include <stdlib.h> /* also for srand, rand */
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <cmath>
#include <time.h>       /* time */
#include <random>

#include "PlayerAlgorithm.h"
#include "RPS_Move.h"
#include "RPS_JokerChange.h"
#include "RPS_PiecePosition.h"
#include "RPS_ParserMove.h"
#include "RPS_ParserPosition.h"
#include "RPS_Board.h"

#define M_COLUMNS 10
#define N_ROWS 10
#define R_MAX 2
#define P_MAX 5
#define S_MAX 1
#define B_MAX 2
#define J_MAX 2
#define F_MAX 1

#define RANDOM_TRIES 3
#define SEED_VAR 7

using std::string;
using std::ifstream;
using std::cout;
using std::endl;
using std::unique_ptr;


//--------------------------

/*
 * reason for ifstream moveFile:
 * the moves file will open when "getInitialPositions" is called with the player number.
 * this way, in each turn of the filealgorithm we will read the next line from the moves file.
 * the d'tor of "FilePlayerAlgorithm" will close the moves file.
 * AGREED BY AMIR in 3.5.2018
 */

class AutoPlayerAlgorithm: public PlayerAlgorithm {
	int player;
	int opponent;
	RPS_Board board;
	Piece last_move_piece;

	void findRandomClearPosition(RPS_Point& point);
	void updateBoardandVecotr(	int player,
								std::vector<unique_ptr<PiecePosition>>& vectorToFill,
								int number_of_pieces,
								char piece_type);
	int getMovefindToPoint(const RPS_Point& from_point, RPS_Point& to_point);
	int getMovefindOffensiveToPoint(const RPS_Point& from_point, RPS_Point& to_point);

public:
	//AutoPlayerAlgorithm();

	virtual void getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill)override;
	virtual void notifyOnInitialBoard(const Board& b, const std::vector<unique_ptr<FightInfo>>& fights)override;
	virtual void notifyOnOpponentMove(const Move& move)override; // called only on opponents move
	virtual void notifyFightResult(const FightInfo& fightInfo)override; // called only if there was a fight
	virtual unique_ptr<Move> getMove()override;
	virtual unique_ptr<JokerChange> getJokerChange()override; // nullptr if no change is requested


	//~AutoPlayerAlgorithm() {}

};

#endif /* AUTOPLAYERALGORITHM_H_ */

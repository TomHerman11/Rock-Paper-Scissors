/*
 * FilePlayerAlgorithm.h
 *
 *  Created on: Apr 29, 2018
 *      Author: tomhe
 */

#ifndef FILEPLAYERALGORITHM_H_
#define FILEPLAYERALGORITHM_H_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <cmath>

#include "PlayerAlgorithm.h"
#include "RPS_Move.h"
#include "RPS_JokerChange.h"
#include "RPS_PiecePosition.h"
#include "RPS_ParserMove.h"
#include "RPS_ParserPosition.h"

using std::string;
using std::ifstream;
using std::cout;
using std::endl;
using std::unique_ptr;

#define POISTION_FILE1 "player1.rps_board"
#define POISTION_FILE2 "player2.rps_board"
#define MOVE_FILE1 "player1.rps_moves"
#define MOVE_FILE2 "player2.rps_moves"
#define OUTPUT_FILE "rps.output"
#define LINE_POSITION_ARGS 4

//--------------------------

/*
 * reason for ifstream moveFile:
 * the moves file will open when "getInitialPositions" is called with the player number.
 * this way, in each turn of the filealgorithm we will read the next line from the moves file.
 * the d'tor of "FilePlayerAlgorithm" will close the moves file.
 * AGREED BY AMIR in 3.5.2018
 */

class FilePlayerAlgorithm: public PlayerAlgorithm {
	int player;
	ifstream moveFile;
	RPSParserMove moveParser;

public:
	//FilePlayerAlgorithm(): player(0), moveFile(), moveParser() {}

	virtual void getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill)override;
	virtual unique_ptr<Move> getMove()override;
	virtual unique_ptr<JokerChange> getJokerChange()override; // nullptr if no change is requested
	~FilePlayerAlgorithm() {if (moveFile.is_open()) moveFile.close();}

	//@@@@@@@@@@@not in use in FILEalgorithm@@@@@@@@@@@
	virtual void notifyOnInitialBoard(const Board& b, const std::vector<unique_ptr<FightInfo>>& fights)override;
	virtual void notifyOnOpponentMove(const Move& move)override; // called only on opponent’s move
	virtual void notifyFightResult(const FightInfo& fightInfo)override; // called only if there was a fight

};

#endif /* FILEPLAYERALGORITHM_H_ */

/*
 * FilePlayerAlgorithm.cpp
 *
 *  Created on: Apr 29, 2018
 *      Author: tomhe
 */

#include "FilePlayerAlgorithm.h"

/*
 * check if line holds any char except white spaces.
 */
int checkEmptyLine(const string& line){
	string word;
	std::istringstream st(line);
	st>>word;
	if(word.empty()){
		return 1; //line is empty
	}
	return 0; //line is not empty
}

void FilePlayerAlgorithm::getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill) {
	//move file will be opened at the end of the function. details explained in the header file.
	//save player
	this->player=player;
	//open positions file
	string position_path;
	if (player==1) {
		position_path=POISTION_FILE1;
	}
	else if (player==2) {
		position_path=POISTION_FILE2;
	}
	ifstream posFile(position_path);
	if (!posFile.is_open()) { //could not open file!
		return;
	}

	//read lines from the file
	string buffer;
	RPS_ParserPosition parsrPosition;


	while (std::getline(posFile, buffer)) {
		if (checkEmptyLine(buffer)) {
			continue;
		}
		parsrPosition=parseLinePosition(buffer);
		if(parsrPosition.getLegalFormat()) {
			vectorToFill.push_back(make_unique<RPS_PiecePosition>(RPS_PiecePosition(parsrPosition.getPieceChar(),
																					parsrPosition.getJokerPieceType(),
																					parsrPosition.getColumnCoor(),
																					parsrPosition.getRowCoor())));

		}
		else{
			vectorToFill.push_back(make_unique<RPS_PiecePosition>(RPS_PiecePosition(0,0,0,0)));
		}
	}
	posFile.close();

	//open move file
	string move_path;
	if (player==1) {
		move_path=MOVE_FILE1;
	}
	else if (player==2) {
		move_path=MOVE_FILE2;
	}
	moveFile.open(move_path.c_str()); //we want to open the move file here since positioning might be correct.
	//if the movefile cannot be opened, getMove will return an invalid move.

}

unique_ptr<Move> FilePlayerAlgorithm::getMove() {
	unique_ptr<RPS_Move> ptr;
	if (!moveFile.is_open()) { //could not open file!
		ptr=make_unique<RPS_Move>(RPS_Move(-1,-1,-1,-1));
		return ptr;
	}

	if (moveFile.eof()) { //end of moves file. returning invalid move
		ptr=make_unique<RPS_Move>(RPS_Move(-1,-1,-1,-1));
		return ptr;
	}

	string line;
	while (std::getline(moveFile, line)) {
		if (checkEmptyLine(line)) {
			continue;
		}
		else {
			break;
		}
	}
	this->moveParser=parseLine(line);
	ptr=make_unique<RPS_Move>(RPS_Move(moveParser.getFromCol(),
										moveParser.getFromRow(),
										moveParser.getToCol(),
										moveParser.getToRow()));
	return ptr;
}

unique_ptr<JokerChange> FilePlayerAlgorithm::getJokerChange() {
	unique_ptr<RPS_JokerChange> ptr;
	if (moveFile.eof()) { //end of moves file. returning invalid jokerchange
		ptr=make_unique<RPS_JokerChange>(RPS_JokerChange(-1,-1,0));
		return ptr;
	}
	if(!moveParser.isJokerChanged()){
		return ptr;
	}
	ptr=make_unique<RPS_JokerChange>(RPS_JokerChange(moveParser.getJokerCol(),
													 moveParser.getJokerRow(),
													 moveParser.getJokerNewRep()));
	return ptr;
}

//@@@@@@@@@@@not in use in FILEalgorithm@@@@@@@@@@@
/*
 * approved by Amir in the class of 10.5.18 since there's a WERROR UNUSED.
 */


void FilePlayerAlgorithm::notifyOnInitialBoard(const Board& b, const std::vector<unique_ptr<FightInfo>>& fights) {
	(void)b;
	(void)fights;
}

void FilePlayerAlgorithm::notifyOnOpponentMove(const Move& move) {
	(void)move;

}
void FilePlayerAlgorithm::notifyFightResult(const FightInfo& fightInfo) {
	(void)fightInfo;
}


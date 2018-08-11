/*
 * main.cpp
 *
 *  Created on: Apr 3, 2018
 *      Author: tomhe
 */

#include "RPSGame.h"


#define POISTION_FILE1 "player1.rps_board"
#define POISTION_FILE2 "player2.rps_board"
#define MOVE_FILE1 "player1.rps_moves"
#define MOVE_FILE2 "player2.rps_moves"
#define OUTPUT_FILE "rps.output"

int main(int argc, char* argv[]) {
	if(argc &&argv){

	}
	RPSGame game;
	LosingDetails losingdetails;

	if (!movesFilesExist(MOVE_FILE1, MOVE_FILE2)) {
		return 1;
	}
	game=RPSGame();
	int init_result=game.initRPSGame(losingdetails, POISTION_FILE1, POISTION_FILE2);
	if (init_result==-1) { //error occurred in opening one of the files. no winner/loser.
		return 1;
	}

	if (init_result==0) { //there's a winner - one of the position files is not valid. a output file has been made.
		writeToOutputFile(OUTPUT_FILE, game, losingdetails);
		return 0;
	}

	//else - continue to move files.
	if(game.executeGame(MOVE_FILE1, MOVE_FILE2, losingdetails)==0){//if there is a problem during the execution of the game
		return 1;
	}
	else{
		writeToOutputFile(OUTPUT_FILE, game, losingdetails);
	}
	return 0;
}

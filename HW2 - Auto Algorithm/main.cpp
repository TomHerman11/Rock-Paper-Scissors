/*
 * main.cpp
 *
 *  Created on: Apr 29, 2018
 *      Author: tomhe
 */

using namespace std;

#include <time.h>       /* time */
#include <random>

#include "RPS_Manager.h"
#include "FilePlayerAlgorithm.h"
#include "AutoPlayerAlgorithm.h"


int main(int argc, char* argv[]) {
	if (argc!=2) return 1;

	//assuming command line arg is valid
	string str_player1(argv[1], argv[1]+4);
	string str_player2(argv[1]+8, argv[1]+12);
	unique_ptr<PlayerAlgorithm> player1;
	unique_ptr<PlayerAlgorithm> player2;

	if(str_player1=="file"){
		player1=make_unique<FilePlayerAlgorithm>();
	}
	else if(str_player1=="auto"){
		player1=make_unique<AutoPlayerAlgorithm>();
	}

	if(str_player2=="file"){
		player2=make_unique<FilePlayerAlgorithm>();
	}
	else if(str_player2=="auto"){
		player2=make_unique<AutoPlayerAlgorithm>();
	}
	Manager game_manager;
	return game_manager.game_manager(*(player1.get()), *(player2.get()));

}

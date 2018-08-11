/*
 * main.cpp
 *
 *  Created on: Apr 29, 2018
 *      Author: tomhe
 */

using namespace std;

#include "RPS_Manager_ex3.h"
#include "RPS_TournamentManager.h"


int main(int argc, char* argv[]) {
	int threads_num=4;
	string path=".";

	if(argc>=3){
		if ((string)argv[1]=="threads") {
			threads_num=atoi(argv[2]);
		}
		else if((string)argv[1]=="path"){
			path=argv[2];
		}

		if(argc==5){
			if((string)argv[3]== "threads"){
				threads_num=atoi(argv[4]);
			}
			else if((string)argv[3]== "path"){
				path=argv[4];
			}
		}
	}
	return RPS_TournamentManager::getTournamentManager().manage(threads_num, path);
}

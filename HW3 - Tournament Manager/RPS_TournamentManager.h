/*
 * RPS_TournamentManager.h
 *
 *  Created on: May 24, 2018
 *      Author: krimolovsky
 */

#ifndef RPS_TOURNAMENTMANAGER_H_
#define RPS_TOURNAMENTMANAGER_H_

#include <functional>
#include <memory>
#include <iostream>

#include <map>
//#include "PlayerAlgorithm.h"
#include "RPS_Manager_ex3.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

//check what we need- taken from main file for so
#include <dlfcn.h>
#include <unistd.h>
#include <string.h>

//for random_shuffle:
#include <algorithm>    // std::random_shuffle
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand


#define SO_FILES_BUFF_SIZE 1024
#define GAMES_TO_PLAY 30

class RPS_idDetails {
	const std::string id;
	std::atomic<int> score;
	int games_played;

public:
	RPS_idDetails(std::string _id): id(_id), score(0), games_played(0) {}
	string getId() {return id;}
	int getGamesPlayed() {return games_played;}
	int getScore(){return score;}
	void increaseOneGamesPlayed() {games_played++;}
	void updateScore(int points) {score+=points;}
	void printIdDetails(){cout<<id<<" "<<score<<endl;}
	//void printIdDetails(){cout<<id<<" "<<score<<" "<<games_played<<endl;} //change back to the previous func
};


class BattlesPool{
	vector<int> battlesVec;
	int curr_i;
	int size;

public:
	void shuffleVector();

	BattlesPool(): curr_i(0), size(0) {} //c'tor
	void changeBattlesPoolVals(int _size);
	std::pair<int, int> getPair();
};

class RPS_TournamentManager {
	static RPS_TournamentManager tournament_manager;

	vector<void *> libs;
	std::map<std::string, std::function<std::unique_ptr<PlayerAlgorithm>()>> id2factory;
	std::vector<shared_ptr<RPS_idDetails>> details_vector;
	BattlesPool battlesPool;
	std::mutex mng_mutex;
	std::condition_variable mng_cv;
	int ready_to_search;


    RPS_TournamentManager(): ready_to_search(1) {} // private ctor
public:
    /*
     * insert to vector details of id, score, and games played. used throughout the "manage" function (to update scores and check how many games played).
     */
    void initDetailsVector();
    void initBattlesPool(int size){battlesPool.changeBattlesPoolVals(size); battlesPool.shuffleVector();}


    int manage(int threads_num, string path);
    void runTournamentThread();
    void updateScore(int player_index, int score);
    std::pair<int, int> getPairForBattle();
    void printResults();

    //if no path was givan, the function receives path="."
    int loadPlayersAlgorithms(string path);

    void registerAlgorithm(string id, function<unique_ptr<PlayerAlgorithm>()> factoryMethod);
    static RPS_TournamentManager& getTournamentManager();
    void closeLibs();
};


#endif /* RPS_TOURNAMENTMANAGER_H_ */

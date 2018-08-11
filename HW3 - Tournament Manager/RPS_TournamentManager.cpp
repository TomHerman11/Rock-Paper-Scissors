/*
 * RPS_TournamentManager.cpp
 *
 *  Created on: May 24, 2018
 *      Author: krimolovsky
 */

#include "RPS_TournamentManager.h"

RPS_TournamentManager RPS_TournamentManager::tournament_manager;

RPS_TournamentManager& RPS_TournamentManager::getTournamentManager(){
	return tournament_manager;
}

int RPS_TournamentManager::loadPlayersAlgorithms(string path){
	FILE *dl;   // handle to read directory
	stringstream stream_command_str;

	int path_length= path.length();
	if(path[path_length-1]=='/'){
		stream_command_str<<"ls "<<path<<"*.so";
	}
	else{
		stream_command_str<<"ls "<<path<<"/*.so";
	}
	string command_str=stream_command_str.str();

	char in_buf[SO_FILES_BUFF_SIZE]; // input buffer for lib names

	// get the names of all the dynamic libs (.so  files) in the current dir
	dl = popen(command_str.c_str(), "r");
	if(!dl){
		perror("Error getting so files in the given path");
		return -1;
	}
	void *dlib;
	while(fgets(in_buf, SO_FILES_BUFF_SIZE, dl)){
		// trim off the whitespace
		char *ws = strpbrk(in_buf, " \t\n");
		if(ws){
			*ws = '\0';
		}

		stringstream stream_player_algo_path;
		//stream_player_algo_path<<path<<"/"<<in_buf;
		stream_player_algo_path<<in_buf;
		string player_algo_path=stream_player_algo_path.str();


		dlib = dlopen(player_algo_path.c_str(), RTLD_NOW);
		if(dlib == NULL){
			cout<<"Error opening "<<player_algo_path.c_str()<<endl;
		}
		else{
			libs.push_back(dlib);
		}
	}
	pclose(dl);


	return 0;
}

void RPS_TournamentManager::closeLibs(){
	int size= libs.size();

	tournament_manager.id2factory.clear();

	for(int i=0; i<size; i++) {
		dlclose(libs[i]);
	}
}

void RPS_TournamentManager::registerAlgorithm(string id, function<unique_ptr<PlayerAlgorithm>()> factoryMethod) {
	std::map<std::string, std::function<std::unique_ptr<PlayerAlgorithm>()>>::iterator it=id2factory.find(id);
	if (it!=id2factory.end()) { //ID already in map!!
		cout << "RSP_" << id << "is already uploaded to the tournament!" << endl;
	}
	else { //ID not in the map. insert it.
		id2factory[id]=factoryMethod;
	}
}

void RPS_TournamentManager::initDetailsVector() {
	//iterate on map, insert ids and game played and score to 0.
	for(std::map<std::string, std::function<std::unique_ptr<PlayerAlgorithm>()>>::iterator it = id2factory.begin(); it != id2factory.end(); ++it) {
		details_vector.push_back(make_shared<RPS_idDetails>(it->first));
	}
}

int RPS_TournamentManager::manage(int threads_num, string path){

	if(loadPlayersAlgorithms(path)==-1){
		return -1;
	}

	if(tournament_manager.id2factory.size()==0){
		cout<<"No so files registered"<<endl;
		closeLibs();
		return 0;
	}
	if(tournament_manager.id2factory.size()==1){
		cout<<"Only one so file registered"<<endl;
		closeLibs();
		return 0;
	}

	//define vector of ids and how many game the id played - the map aleady lodaed with id-s and algorithms
	initDetailsVector();

	//TODO: init all class fields- currently no constructor
	initBattlesPool(details_vector.size());

	vector<std::thread> threads_vec;


	for(int i=0; i<threads_num-1; i++){
		threads_vec.push_back(std::thread(&RPS_TournamentManager::runTournamentThread, this));
	}
	runTournamentThread();//main thread calling this function

	for(auto& curr_thread: threads_vec){
		curr_thread.join();
	}

	printResults();
	closeLibs();
	return 0;
}

void RPS_TournamentManager::runTournamentThread(){
	string id1;
	string id2;
	int winner;
	int curr_player1;
	int curr_player2;


	while(1){//WHILE THERE ARE MORE GAMES TO PLAY
		std::pair<int, int> rivals= this->getPairForBattle();
		curr_player1=rivals.first;
		curr_player2= rivals.second;
		if(curr_player1==-1 && curr_player2==-1){
			break;
		}
		id1=details_vector[curr_player1]->getId();
		id2=details_vector[curr_player2]->getId();
		auto func_id1=id2factory[id1];
		auto func_id2=id2factory[id2];



		unique_ptr<PlayerAlgorithm> player1= func_id1();
		unique_ptr<PlayerAlgorithm> player2= func_id2();

		Game_Manager game_mng;
		winner= game_mng.game_manager(*(player1.get()), *(player2.get()));
		//update score
		switch(winner){
		case 1:
		//	cout << "winner: " << id1 << endl;
			updateScore(curr_player1, 3);
			break;
		case 2:
		//	cout << "winner: " << id2 << endl;
			updateScore(curr_player2, 3);
			break;
		case 0:
			//cout << "tie!" << endl;
			updateScore(curr_player1, 1);
			updateScore(curr_player2, 1);
			break;
		}
	//	cout << "id1: " << id1 << " id2: " << id2 << endl;
		//cout<<std::this_thread::get_id() << endl;
	}
	return;
}

void RPS_TournamentManager::updateScore(int player_index, int score){
	details_vector[player_index]->updateScore(score);
}

void BattlesPool::shuffleVector(){
	std::random_shuffle(battlesVec.begin(), battlesVec.end());
	/*cout<<"shuffle before fixing: ";
	for(int i=0; i<size; i++){
		cout<<battlesVec[i]<<", ";
	}
	cout<<endl;*/

	for(int i=0; i<size; i++){
		if(battlesVec[i]==i){
			battlesVec[i]= battlesVec[(i+1)%size];
			battlesVec[(i+1)%size]= i;
		}
	}
/*	cout<<"shuffle fixed: ";
	for(int i=0; i<size; i++){
		cout<<battlesVec[i]<<", ";
	}
	cout<<endl;*/
}

void BattlesPool::changeBattlesPoolVals(int _size) {
	curr_i=0;
	size=_size;
	for(int i=0; i<size; i++){
		battlesVec.push_back(i);
	}
}

std::pair<int, int> BattlesPool::getPair(){
	if(curr_i==size){
		shuffleVector();
		curr_i=0;
	}

	curr_i++;
	return std::make_pair(curr_i-1, battlesVec[curr_i-1]);
}

std::pair<int, int> RPS_TournamentManager::getPairForBattle(){
	static unsigned int finished_players=0;
	std::pair<int, int> rivals;

	unique_lock<mutex> lk(mng_mutex);
	//TODO: need to init ready_to_search to 1 before first calling this function
	mng_cv.wait(lk, [this]{return ready_to_search;});
	ready_to_search=0;

	if(finished_players==details_vector.size()){
		rivals= std::make_pair(-1, -1);
	}
	else{
		rivals=battlesPool.getPair();
		if(details_vector[rivals.first]->getGamesPlayed()== GAMES_TO_PLAY-1){
			finished_players++;
		}
		details_vector[rivals.first]->increaseOneGamesPlayed();

		if(details_vector[rivals.second]->getGamesPlayed()== GAMES_TO_PLAY-1){
			finished_players++;
		}
		details_vector[rivals.second]->increaseOneGamesPlayed();
	}

	ready_to_search=1;
	mng_cv.notify_one();
	lk.unlock();
	return rivals;

}

void RPS_TournamentManager::printResults(){
	std::sort(details_vector.begin(), details_vector.end(), [](const shared_ptr<RPS_idDetails>& details1,
				const shared_ptr<RPS_idDetails>& details2){return details2->getScore()< details1->getScore();});

	for(unsigned int i=0; i<details_vector.size(); i++){
		details_vector[i]->printIdDetails();
	}
}

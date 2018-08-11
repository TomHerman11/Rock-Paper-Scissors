/*
 * AutoPlayerAlgorithm.cpp
 *
 *  Created on: May 7, 2018
 *      Author: tomhe
 */

#include "AutoPlayerAlgorithm.h"
//@@@@@@@@@aux functions@@@@@@@@@
int validPoistiononBoard(const Point& point) {
	int col=point.getX();
	int row=point.getY();

	if ((col<1 || col>M_COLUMNS) || (row<1 || row>N_ROWS)) {
		return 0;
	}
	return 1;
}

/*
 * return 1 if type_player is stronger.
 * return 2 if type_opponent is stronger.
 */
int findStrongerTypeOffensive(char type_player, char type_opponent) {
	if (type_player==type_opponent) {
		return 0; //both sides are dead
	}

	//type_player winning states
	if ((type_player=='R' && type_opponent=='S') || (type_player=='S' && type_opponent=='P') || (type_player=='P' && type_opponent=='R')) {return 1;}

	//type_opponent winning states
	if ((type_opponent=='R' && type_player=='S') || (type_opponent=='S' && type_player=='P') || (type_opponent=='P' && type_player=='R')) {return 2;}

	return 0;
}

//@@@@@@@@@private functions in AutoPlayerAlgorithm@@@@@@@@@
void AutoPlayerAlgorithm::findRandomClearPosition(RPS_Point& point) {
	//WILL TRY "RANDOM_TRIES" TO FIND A RANDOM POINT. IF NOT SUCCESSEDED, WILL SEARCH ONE BY ONE

	/* generate secret number between 1 and N_ROWS: */
	int row_index=0;
	int col_index=0;
	RPS_Point tmp_point;

	for (int i=0; i<RANDOM_TRIES; i++ ) {
		row_index=1+(rand()%N_ROWS);
		col_index=1+(rand()%M_COLUMNS);


		tmp_point=RPS_Point(col_index, row_index);
		if (board.getPlayer(tmp_point)==0) { //blank square
			point=tmp_point;
			return;
		}
	}

	//if we get to here - we didn't find a blank square by RANDOM_TRIES tries.
	for (int i=1; i<=N_ROWS; i++) {
		for (int j=1; j<=M_COLUMNS; j++) {
			if (board.getPlayer(RPS_Point(j,i))==0) {
				point=RPS_Point(j,i);
				return;
			}
		}
	}
}

void AutoPlayerAlgorithm::updateBoardandVecotr(	int player,
												std::vector<unique_ptr<PiecePosition>>& vectorToFill,
												int number_of_pieces,
												char piece_type)
{
	char type_for_inside_board=piece_type;
	char joker_rep='#';
	int is_joker=0;
	if (piece_type=='J') {
		is_joker=1;
		type_for_inside_board='B';
		joker_rep='B'; //TO BE CHANGED LATER?
	}
	RPS_Point tmp_point;
	Piece tmp_piece;

	for (int i=0; i<number_of_pieces; i++) {
		findRandomClearPosition(tmp_point);
		if (tmp_point.getX()==0 && tmp_point.getY()==0) {
			//no more space to insert pieces
			return;
		}
		tmp_piece=Piece(type_for_inside_board, is_joker, player);
		board.setPieceInPos(tmp_piece, tmp_point);
		vectorToFill.push_back(make_unique<RPS_PiecePosition>(RPS_PiecePosition(piece_type,
																				joker_rep,
																				tmp_point.getX(),
																				tmp_point.getY())));
	}
}

int AutoPlayerAlgorithm::getMovefindToPoint(const RPS_Point& from_point, RPS_Point& to_point) {
	int from_col=from_point.getX();
	int from_row=from_point.getY();
	RPS_Point tmp_point;

	if (!(board.getPiece(from_point)).isMovingPiece()) {
		return 0;
	}

	//look up
	tmp_point=RPS_Point(from_col, from_row+1);
	if (board.getPlayer(tmp_point)!=player && validPoistiononBoard(tmp_point)==1) {
		to_point=tmp_point;
		return 1;
	}

	//look down
	tmp_point=RPS_Point(from_col, from_row-1);
	if (board.getPlayer(tmp_point)!=player && validPoistiononBoard(tmp_point)==1) {
		to_point=tmp_point;
		return 1;
	}

	//look right
	tmp_point=RPS_Point(from_col+1, from_row);
	if (board.getPlayer(tmp_point)!=player && validPoistiononBoard(tmp_point)==1) {
		to_point=tmp_point;
		return 1;
	}

	//look left
	tmp_point=RPS_Point(from_col-1, from_row);
	if (board.getPlayer(tmp_point)!=player && validPoistiononBoard(tmp_point)==1) {
		to_point=tmp_point;
		return 1;
	}
	return 0;
}

int AutoPlayerAlgorithm::getMovefindOffensiveToPoint(const RPS_Point& from_point, RPS_Point& to_point) {
	int from_col=from_point.getX();
	int from_row=from_point.getY();
	char player_type=board.getPiece(from_point).getType();
	char opponent_type;
	int findStringerResult=0;
	int ret_val=0;

	RPS_Point possible_points[]={RPS_Point(from_col, from_row+1), RPS_Point(from_col, from_row-1), RPS_Point(from_col+1, from_row), RPS_Point(from_col-1, from_row)};
	for (int i=0; i<4; i++) {
		if (board.getPlayer(possible_points[i])==opponent && validPoistiononBoard(possible_points[i])==1) {
			opponent_type=board.getPiece(possible_points[i]).getType();
			findStringerResult= findStrongerTypeOffensive(player_type, opponent_type);
			if (findStringerResult==1) {
				to_point=possible_points[i];
				return 1;
			}
			if (findStringerResult==0) {
				to_point=possible_points[i];
				ret_val=2;
			}
		}
	}
	return ret_val;
}

//@@@@@@@@@public functions in AutoPlayerAlgorithm@@@@@@@@@
void AutoPlayerAlgorithm::getInitialPositions(	int player,
												std::vector<unique_ptr<PiecePosition>>& vectorToFill)
{

	srand(time(NULL)+player+SEED_VAR);
	this->player=player;
	this->opponent=(player==1) ? 2:1;
	//POSITIONING IS RANDOM
	updateBoardandVecotr(player, vectorToFill, F_MAX, 'F');

	updateBoardandVecotr(player, vectorToFill, R_MAX, 'R');
	updateBoardandVecotr(player, vectorToFill, P_MAX, 'P');
	updateBoardandVecotr(player, vectorToFill, S_MAX, 'S');

	updateBoardandVecotr(player, vectorToFill, B_MAX, 'B');
	updateBoardandVecotr(player, vectorToFill, J_MAX, 'J');
}

void AutoPlayerAlgorithm::notifyOnInitialBoard(const Board& b, const std::vector<unique_ptr<FightInfo>>& fights){
	FightInfo *fightInfo_ptr;
	int k=0;
	int vector_size=fights.size();
	Piece tmpPiece;

	while (k<vector_size) {
		fightInfo_ptr=fights[k].get();
		if(fightInfo_ptr->getWinner()==opponent){
			tmpPiece= Piece(fightInfo_ptr->getPiece(opponent),0, opponent);
			board.setPieceInPos(tmpPiece, fightInfo_ptr->getPosition());
		}
		if (fightInfo_ptr->getWinner()==0) {
			board.setEmptyPieceInPos(fightInfo_ptr->getPosition());
		}
		k++;
	}

	RPS_Point pos;
	for(int i=1; i<=M_COLUMNS; i++){ //columns
		for(int j=1; j<=N_ROWS; j++){//rows
			pos= RPS_Point(i,j);
			if(board.getPlayer(pos)==0 && b.getPlayer(pos)==opponent){
				tmpPiece= Piece(0 ,0, opponent);
				board.setPieceInPos(tmpPiece, pos);
			}
		}
	}
}

void AutoPlayerAlgorithm::notifyOnOpponentMove(const Move& move){
	if(board.getPlayer(move.getTo())== player){ //there will be fight
		Piece moved_piece= board.getPiece(move.getTo());
		last_move_piece=moved_piece;
	}
	Piece tmp_piece=board.getPiece(move.getFrom());
	board.setPieceInPos(tmp_piece, move.getTo());

	board.setEmptyPieceInPos(move.getFrom());
}

void AutoPlayerAlgorithm::notifyFightResult(const FightInfo& fightInfo){
	int winner=fightInfo.getWinner();
	if (winner==0){
		board.setEmptyPieceInPos(fightInfo.getPosition());
		last_move_piece=Piece();
		return;
	}
	if (winner==player){
		board.setPieceInPos(last_move_piece, fightInfo.getPosition());
		return;
	}

	else{ //opponent wins
		//see if we can know if the opponent's piece is a joker
		char prev_type= board.getPiece(fightInfo.getPosition()).getType(); //the opponent's known or unknown type
		char type_opponent=fightInfo.getPiece(opponent);
		if(prev_type!=0 && prev_type!=type_opponent) {
			Piece tmpPiece=Piece(type_opponent, 1, opponent);
			board.setPieceInPos(tmpPiece, fightInfo.getPosition());
		}
		else {
			board.getPiece(fightInfo.getPosition()).setType(type_opponent);
		}
	}
}


unique_ptr<Move> AutoPlayerAlgorithm::getMove() {
	RPS_Point from_point;
	RPS_Point to_point;
	RPS_Point curr_point;
	unique_ptr<RPS_Move> ptr;
	Piece tmp_piece;

	int find_offensive=0;
	int find_naive_flag=1;
	int move_found=0;
	int find_offensive_result=0;


	//find from point
	for (int i=1; i<=N_ROWS; i++) {
		for (int j=1; j<=M_COLUMNS; j++) {
			curr_point=RPS_Point(j,i);
			if (!(board.getPiece(curr_point)).isMovingPiece()) {
				continue;
			}
			if (board.getPlayer(curr_point)!=player) {
				continue;
			}
			find_offensive_result= getMovefindOffensiveToPoint(curr_point, to_point);
			if (find_offensive_result) { //an offensive move. we'll take it. to_point was updated.
				from_point=curr_point;
				move_found=1;
				find_naive_flag=0;
				if(find_offensive_result==1){
					find_offensive=1;
					break;
				}
			}

			if(find_naive_flag){
				if (getMovefindToPoint(curr_point, to_point)==1){ //to_point was updated.
					from_point= curr_point;
					find_naive_flag=0; //no need to check for naive moves anymore.
					move_found=1;
					break;
				}
			}
		}
		if (find_offensive==1) {break;} //exit second loop
		//if(move_found==1){break;}
	}
	if(move_found){
		ptr=make_unique<RPS_Move>(RPS_Move(from_point, to_point));
		if(board.getPlayer(to_point)== opponent){ //there will be a fight, and then we will update the winner
		/*	board.printBoard();
			cout << "board player: " << board.getPlayer(from_point);
			char mychar=board.getPiece(from_point).getType();
			cout << " " << mychar << ": ";
			cout << "from_x: " << from_point.getX() << " from_y: " << from_point.getY() << " to_x: " << to_point.getX() << " to_y: " << to_point.getY() << endl;
		*/
			Piece moved_piece=board.getPiece(from_point);
			last_move_piece=moved_piece;
		}
		else {
			tmp_piece=board.getPiece(from_point);
			board.setPieceInPos(tmp_piece, to_point);
		}
		board.setEmptyPieceInPos(from_point);
		return ptr;
	}

	//if we get to here, no available moves for any of the pieces!
	ptr=make_unique<RPS_Move>(RPS_Move(-1,-1,-1,-1));
	return ptr;
}

unique_ptr<JokerChange> AutoPlayerAlgorithm::getJokerChange() {
	unique_ptr<JokerChange> ptr;
	return ptr;
}









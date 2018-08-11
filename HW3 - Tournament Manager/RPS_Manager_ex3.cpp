/*
 * RPS_Manager.cpp
 *
 *  Created on: Apr 29, 2018
 *      Author: tomhe
 */

#include "RPS_Manager_ex3.h"

//@@@@@@@@@@AUX FUNCTIONS@@@@@@@@@@
int checkValidPieceType(char piece_char, int pieces_counter[]) {
	if (piece_char=='R') {
		pieces_counter[0]++;
	}
	else if (piece_char=='P') {
		pieces_counter[1]++;
	}
	else if (piece_char=='S') {
		pieces_counter[2]++;
	}
	else if (piece_char=='B') {
		pieces_counter[3]++;
	}
	else if (piece_char=='F') {
		pieces_counter[4]++;
	}
	else if (piece_char=='J') {
		pieces_counter[5]++;
	}
	else { //not valid value to represent PIECE CHAR
		return 0;
	}
	return 1;
}

int checkValidJokerRep(char joker_rep) {
	if(joker_rep=='R' || joker_rep=='P' || joker_rep=='S'|| joker_rep=='B'){
		return 1;
	}
	return 0; //illegal joker representation
}

int checkValidPosition(const Point& point) {
	int col=point.getX();
	int row=point.getY();

	if ((col<1 || col>M_COLUMNS) || (row<1 || row>N_ROWS)) {
		return 0;
	}
	return 1;
}

int checkValidPiecesBound(int pieces_counter[]) {
	//index: 0 - R, 1 - P, 2 - S, 3 - B, 4 - F, 5 - J
	if (pieces_counter[0]>R_MAX ||
		pieces_counter[1]>P_MAX ||
		pieces_counter[2]>S_MAX ||
		pieces_counter[3]>B_MAX ||
		pieces_counter[4]>F_MAX ||
		pieces_counter[5]>J_MAX)
	{
	return 0;
	}
	return 1;
}

int isLegalInputBoardFile(vector<unique_ptr<PiecePosition>>& vectorPositions){
	int piecesPositions[N_ROWS][M_COLUMNS]={0}; //we fill this matrix with 0's. change to 1 if a piece has been placed in this coordinates.
	int pieces_counter[6]={0}; //index: 0 - R, 1 - P, 2 - S, 3 - B, 4 - F, 5 - J

	//vector<unique_ptr<PiecePosition>>::iterator it;
	PiecePosition* tmp_piece_ptr;

	int i=0;
	int vector_size=vectorPositions.size();

	while (i<vector_size) {
		tmp_piece_ptr=vectorPositions[i].get();
		if (!checkValidPieceType(tmp_piece_ptr->getPiece(), pieces_counter)) {
			//if we get here - piece type is not R || P || S || B || F || J
			return 0;
		}

		if (tmp_piece_ptr->getPiece()=='J') {
			if (!checkValidJokerRep(tmp_piece_ptr->getJokerRep())) {
				//if we get here - joker rep is not R || P || S || B
				return 0;
			}
		}

		const Point& point=tmp_piece_ptr->getPosition();
		if (!checkValidPosition(point)) {
			//if we get here - position is out of board limits.
			return 0;
		}

		if (piecesPositions[point.getY()-1][point.getX()-1]==1) {
			//if we get here - two pieces in the same position.
			return 0;
		}
		else {
			piecesPositions[point.getY()-1][point.getX()-1]=1;
		}

		//NEXT PIECEPOSITION
		i++;
	}
	if (!checkValidPiecesBound(pieces_counter)) {
		//if we get here - exceeded pieces bound.
		return 0;
	}
	if (pieces_counter[4]!=F_MAX) {
		//if we get here - too many flags.
		return 0;
	}
	return 1;
}

int fightPieces(const Piece &piece1, const Piece &piece2, char &p1Type, char &p2Type) {
	char type1=piece1.getType();
	char type2=piece2.getType();

	p1Type=type1;
	p2Type=type2;

	if (type1==type2) {
		return 0; //both sides are dead
	}
	if (type1==0) {return 2;}
	if (type2==0) {return 1;}

	if (type1=='B') {return 1;} //bomb is the strongest. will not lose since we checked that types not equal.
	if (type2=='B') {return 2;}

	if (type1=='F') {return 2;} //flag is the weakest. will not win since we checked that types not equal.
	if (type2=='F') {return 1;}

	//piece 1 winning states
	if ((type1=='R' && type2=='S') || (type1=='S' && type2=='P') || (type1=='P' && type2=='R')) {return 1;}

	//piece 2 winning states
	if ((type2=='R' && type1=='S') || (type2=='S' && type1=='P') || (type2=='P' && type1=='R')) {return 2;}

	return 1;
}

int isLegalJokerRep(char rep){
	if(rep=='R' || rep=='P' || rep=='S'|| rep=='B'){
		return 1;
	}
	return 0; //illegal joker representation
}

//@@@@@@@@@@WRITE AUX@@@@@@@@@@

void auxWriteReason(int winning_cause, string& reason, int loser) {
	stringstream reason_stream;

	if (winning_cause==PLAYER_BAD_POSITION) {
		reason_stream << "Bad Positioning input for player " << loser;
	}
	if (winning_cause==TIE_BAD_POSITION) {
		reason_stream << "Bad Positioning input for both players";
	}
	if (winning_cause==PLAYER_FLAGS_EATEN) {
		reason_stream << "All flags of the opponent are captured";
	}
	if (winning_cause==TIE_FLAGS_EATEN) {
		reason_stream << "A tie - all flags are eaten by both players in the position files";
	}
	if (winning_cause==PLAYER_NO_MOVING_PIECES) {
		reason_stream << "All moving PIECEs of the opponent are eaten";
	}
	if (winning_cause==TIE_NO_MOVING_PIECES) {
		reason_stream << "A tie - all moving PIECEs are eaten by both players in the position files";
	}
	if (winning_cause==ILLEGAL_MOVE) {
		reason_stream << "Illegal move for player " << loser;
	}
	if(winning_cause==TOO_MANY_MOVES_WITH_NO_FIGHT){
		reason_stream << "Tie result due to 'no fight' for "<<MAX_MOVES_WITHOUT_FIGHT<<" moves";
	}
	reason=reason_stream.str();
}

//@@@@@@@@@@CLASS MANAGER FUNCTIONS@@@@@@@@@@
/*
void Game_Manager::writetoOutputFile(int winning_cause, int winner) {
	string reason;
	Piece piece;
	RPS_Point tmp_point;
	char pieceType;

	ofstream outFile(OUTPUT_FILE);
	if (!outFile.is_open()) { //could not open file! returns 0.
		cout << "ERROR: could not open output file" << endl;
		return;
	}

	int loser;
	loser=(winner==1) ? 2 : 1;
	if (winner==0) loser=0;
	auxWriteReason(winning_cause, reason, loser);

	outFile << "Winner: " << winner << endl;
	outFile << "Reason: " << reason << endl;
	outFile << endl;

	//cout << "Reason: " << reason << endl;


	//now print the board
	for (int i=1; i<=N_ROWS; i++) {
		for (int j=1; j<=M_COLUMNS; j++) {
			tmp_point=RPS_Point(j, i); //column first and then row
			piece=board.getPiece(tmp_point);
			//piece=game.getPieceRPSGame(i,j);
			pieceType=piece.getType();
			if (piece.getOwnerPlayer()==0) { //"null square"
				outFile << " ";
			}
			//o/w it belongs to some player!
			else if (piece.getOwnerPlayer()==1) {
				if (piece.getIsJoker()) {
					outFile << "J";
				}
				else {
					outFile << pieceType;
				}
			}
			else if (piece.getOwnerPlayer()==2) {
				if (piece.getIsJoker()) {
					outFile << "j";
					continue;
				}
				if (pieceType=='R') {outFile << "r";}
				if (pieceType=='P') {outFile << "p";}
				if (pieceType=='S') {outFile << "s";}
				if (pieceType=='B') {outFile << "b";}
				if (pieceType=='F') {outFile << "f";}
			}
		}
		outFile << endl;
	}
	outFile.close();
}*/

void Game_Manager::increaseGameFieldsUponMoving(const Piece &tmpPiece) {
	int piece_owner=tmpPiece.getOwnerPlayer();
	if (tmpPiece.isMovingPiece()) {
		if (piece_owner==1) {
			movingPiecesPlayer1++;
		}
		else if (piece_owner==2) {
			movingPiecesPlayer2++;
		}
	}
	if (tmpPiece.getType()=='F') {
		if (piece_owner==1) {
			flagsPlayer1++;
		}
		else if (piece_owner==2) {
			flagsPlayer2++;
		}
	}
}

void Game_Manager::reduceGameFieldsUponMoving(const Piece &piece) {
	int piece_owner=piece.getOwnerPlayer();
	if (piece.isMovingPiece()) {
		if (piece_owner==1) {
			movingPiecesPlayer1--;
		}
		else if (piece_owner==2) {
			movingPiecesPlayer2--;
		}
	}
	if (piece.getType()=='F') {
		if (piece_owner==1) {
			flagsPlayer1--;
		}
		else if (piece_owner==2) {
			flagsPlayer2--;
		}
	}
}

void Game_Manager::movePiece(Piece &piece, const Point &point, unique_ptr<FightInfo>& unq_ptr) {
	int winning_piece, winner;
	Piece tmpPiece=Piece(0,0,0);
	if(board.getPlayer(point)==0){
		board.setPieceInPos(piece, point);
		piece=Piece();
		return;
		//return RPS_FightInfo();
	}
	char type1, type2;
	if(piece.getOwnerPlayer()==1){
		winning_piece=fightPieces(piece, board.getPiece(point), type1, type2);
	}
	if(piece.getOwnerPlayer()==2){
		winning_piece=fightPieces(piece, board.getPiece(point), type2, type1);
	}
	//winning_piece=fightPieces(piece, board.getPiece(point), type1, type2);
	switch(winning_piece){
	case 1:
		winner= piece.getOwnerPlayer();
		break;
	case 2:
		winner= board.getPiece(point).getOwnerPlayer();
		break;
	case 0:
		winner=0;
	}
	if(type1=='B' || type2=='B') {
		winner=0;
	}

	unq_ptr=make_unique<RPS_FightInfo>(RPS_FightInfo(point.getX(), point.getY(), type1, type2, winner));

	//fightInfo=RPS_FightInfo(point.getX(), point.getY(), type1, type2,winning_piece);

	if (winning_piece==0) {
		//take care of field of moving pieces
		reduceGameFieldsUponMoving(piece);
		reduceGameFieldsUponMoving(board.getPiece(point));

		board.setPieceInPos(tmpPiece, point);
		//board[row_coor][column_coor]=Piece(0,0,0);

			}
	else if (winning_piece==1) {
		reduceGameFieldsUponMoving(board.getPiece(point));
		board.setPieceInPos(piece, point);
	//	board[row_coor][column_coor]=piece;


	}
	else { //if winning_piece==2, no need for change the board
		reduceGameFieldsUponMoving(piece);
	}

	//if the winning piece is a bomb, we need to erase it now.
	if ((board.getPiece(point)).getType()=='B') {
		//if the bomb is originally a joker - need to update game fields - CHECK IF REALLY NEED TO DO IT
		reduceGameFieldsUponMoving(board.getPiece(point));
		board.setPieceInPos(tmpPiece, point);
		//board[row_coor][column_coor]=Piece(0,0,0);
	}

	//set piece to initial values, like marking "null square" on the game board
	piece=Piece();

	return;
	//return RPS_FightInfo(point.getX(), point.getY(), type1, type2, winning_piece);
}

void Game_Manager::insertPositionPieces(	int player,
									vector<unique_ptr<PiecePosition>>& vectorPositions,
									vector<unique_ptr<FightInfo>>& vectorFights)
	{
	int i=0;
	int vector_size=vectorPositions.size();
	PiecePosition* tmp_piecePosition_ptr;
	Piece tmp_piece;
	//RPS_FightInfo tmp_fightinfo;

	while (i<vector_size) {
		tmp_piecePosition_ptr=vectorPositions[i].get();
		if (tmp_piecePosition_ptr->getPiece()=='J') {
			tmp_piece=Piece(tmp_piecePosition_ptr->getJokerRep(), 1, player);
		}
		else {
			tmp_piece=Piece(tmp_piecePosition_ptr->getPiece(), 0, player);
		}

		//update the game fields (is tmpPiece a moving piece?, a flag?)
		increaseGameFieldsUponMoving(tmp_piece);

		unique_ptr<FightInfo> unq_ptr;
		movePiece(tmp_piece, tmp_piecePosition_ptr->getPosition(), unq_ptr);
		if (unq_ptr!=NULL) {
			vectorFights.push_back(std::move(unq_ptr));
		}

		//insert piece to the board - validity of moves was already checked!
		//tmp_fightinfo=movePiece(tmp_piece, tmp_piecePosition_ptr->getPosition());


		//NEXT PIECEPOSITION
		i++;
	}
}

/*
int Manager::checkWinnerAfterPosition(){
	if (flagsPlayer1==0 && flagsPlayer2==0) { //tie
		losingdetails.setWinnerPlayer(0);
		losingdetails.setLosingCause(LosingCause::TIE_FLAGS_EATEN_POSITION_FILES);
		return 1;
	}

	if (movingPiecesPlayer1==0  && movingPiecesPlayer2==0) { //tie
		losingdetails.setWinnerPlayer(0);
		losingdetails.setLosingCause(LosingCause::MOVING_PIECES_GONE);
		return 1;
	}

	//player 1 loses:
	if (flagsPlayer1==0) {
		losingdetails.setWinnerPlayer(2);
		losingdetails.setLosingCause(LosingCause::All_FLAGS_CAPTURED);
		return 1;
	}

	if (movingPiecesPlayer1==0) {
		losingdetails.setWinnerPlayer(2);
		losingdetails.setLosingCause(LosingCause::MOVING_PIECES_GONE);
		return 1;
	}

	//player2 loses
	if (flagsPlayer2==0) {
		losingdetails.setWinnerPlayer(1);
		losingdetails.setLosingCause(LosingCause::All_FLAGS_CAPTURED);
		return 1;
	}
	if (movingPiecesPlayer2==0) {
		losingdetails.setWinnerPlayer(1);
		losingdetails.setLosingCause(LosingCause::MOVING_PIECES_GONE);
		return 1;
	}
	return 0;
}
*/

/*
int Manager::writetoOutputFileBadPosition(int legal_pos1, int legal_pos2) {
	if (legal_pos1==0 && legal_pos2==0) { //both lose
		writetoOutputFile(TIE_BAD_POSITION, 0);
		return 1;
	}
	if (legal_pos1==1 && legal_pos2==0) {
		writetoOutputFile(PLAYER_BAD_POSITION, 1);
		return 1;
	}
	if (legal_pos1==0 && legal_pos2==1) {
		writetoOutputFile(PLAYER_BAD_POSITION, 2);
		return 1;
	}
	return 0;
}
*/

int Game_Manager::checkWinnerAfterPosition_ex3(){
	//tie:
	if (flagsPlayer1==0 && flagsPlayer2==0) {
		return 0;
	}
	if (movingPiecesPlayer1==0  && movingPiecesPlayer2==0) {
		return 0;
	}

	//player 1 loses:
	if (flagsPlayer1==0) {
		return 2;
	}
	if (movingPiecesPlayer1==0) {
		return 2;
	}

	//player2 loses
	if (flagsPlayer2==0) {
		return 1;
	}
	if (movingPiecesPlayer2==0) {
		return 1;
	}
	return -1; //no winner so far
}

void Game_Manager::changeJokerRep(const JokerChange& jokerChange){
	Piece &joker=board.getPiece(jokerChange.getJokerChangePosition());

	int player= joker.getOwnerPlayer();
	int ismovingRep=(jokerChange.getJokerNewRep()=='B') ? 0 : 1; // if the new rep is of a moving piece=1; else 0;
	int iscurrMovingRep= joker.isMovingPiece();// if the current rep is of a moving piece=1; else 0;
	int movingPiecesChange=0;
	//if the joker changed from a moving rep to a not moving rep, we want to decrease the player's moving pieces
	if(ismovingRep==0 && iscurrMovingRep==1){
		movingPiecesChange= -1;
	}
	//if the joker changed from a not moving rep to a moving rep, we want to increase the player's moving pieces
	else if(ismovingRep==1 && iscurrMovingRep==0){
		movingPiecesChange= 1;
	}
	//change the player's moving pieces counter
	if(player==1){
		movingPiecesPlayer1+= movingPiecesChange;
	}
	if(player==2){
		movingPiecesPlayer2+= movingPiecesChange;
	}
	//change the joker's rep
	joker.setType(jokerChange.getJokerNewRep());
}

int Game_Manager::checkLegalMove(const Move& move, int player) {
	//check if in board limits
	if (!checkValidPosition(move.getFrom()) || !checkValidPosition(move.getTo())) {
		return 0;
	}

	//check if this piece belongs to player
	if (board.getPlayer(move.getFrom())!=player) {
		return 0;
	}

	//check if it's a moving piece
	Piece& tmp_piece=board.getPiece(move.getFrom());
	if (!tmp_piece.isMovingPiece()) {
		return 0;
	}

	//check if there's isn't a player piece in the target point
	if (board.getPlayer(move.getTo())==player) {
		return 0;
	}

	return 1;
}

int Game_Manager::checkLegalJokerChange(const JokerChange& jokerChange, int player) {
	//check if in board limits
	if (!checkValidPosition(jokerChange.getJokerChangePosition())) {
		return 0;
	}

	//check if it's a valid char to change the joker to
	if (!isLegalJokerRep(jokerChange.getJokerNewRep())) {
		return 0;
	}

	//check if this piece belongs to player
	if (board.getPlayer(jokerChange.getJokerChangePosition())!=player) {
		return 0;
	}

	return 1;
}

int Game_Manager::playerHasMovingPieces(int player){
	int result;
	if(player==1){
		result= movingPiecesPlayer1>0;
		return result;
	}
	if(player==2){
		result= movingPiecesPlayer2>0;
		return result;
	}
	return -1;// given player is not legal
}

int Game_Manager::playerHasFlag(int player){
	int result;
	if(player==1){
		result= flagsPlayer1>0;
		return result;
	}
	if(player==2){
		result= flagsPlayer2>0;
		return result;
	}
	return -1;// given player is not legal
}

int Game_Manager::checkWinner(int player){
	int opponent=(player==1)? 2:1;
	if(playerHasFlag(opponent)==0){
		return 1;// opponent has no flags
	}
	if(playerHasMovingPieces(opponent)==0){
		return 1;// opponent has no moving pieces
	}
	return 0; //player didn't win
}

int Game_Manager::game_manager(PlayerAlgorithm& player1, PlayerAlgorithm& player2){
	int legal_pos1;
	int legal_pos2;
	int noFightMoves=0;
	//int sumFights=0;


	std::vector<unique_ptr<PiecePosition>> vectorPositions1;
	std::vector<unique_ptr<PiecePosition>> vectorPositions2;

	//analyzing positions
	player1.getInitialPositions(1, vectorPositions1);
	player2.getInitialPositions(2, vectorPositions2);

	legal_pos1=isLegalInputBoardFile(vectorPositions1);
	legal_pos2=isLegalInputBoardFile(vectorPositions2);

	/*
	if (writetoOutputFileBadPosition(legal_pos1, legal_pos2)==1) {
		//game over. writing to output file has been done
		return 0;
	}
	*/

	//check positioning:
	if (legal_pos1==0 && legal_pos2==0) { //both lose
		return 0;
	}
	if (legal_pos1==1 && legal_pos2==0) {
		return 1;
	}
	if (legal_pos1==0 && legal_pos2==1) {
		return 2;
	}

	//insert pieces into the board
	std::vector<unique_ptr<FightInfo>> vectorFights;
	insertPositionPieces(1, vectorPositions1, vectorFights);
	insertPositionPieces(2, vectorPositions2, vectorFights);

	/*
	if (checkWinnerAfterPosition()==1) {
		writing to output file is done in checkWinnerAfterPosition. we can return now.
		return 0;
	}
	*/

	int check_winner_ret=checkWinnerAfterPosition_ex3();
	if (check_winner_ret!=-1) {
		return check_winner_ret;
	}


	player1.notifyOnInitialBoard(board, vectorFights);
	player2.notifyOnInitialBoard(board, vectorFights);

	//GAME ON!
	PlayerAlgorithm *current_player;
	PlayerAlgorithm *opponent_player;
	current_player=&player1;
	opponent_player=&player2;

	unique_ptr<Move> ptr_move;
	unique_ptr<JokerChange> ptr_jokerChange;
	int player=1;
	int opponent=2;
	while(noFightMoves<MAX_MOVES_WITHOUT_FIGHT) {
		ptr_move=std::move(current_player->getMove());
		if (!checkLegalMove(*(ptr_move.get()), player)) { //game over due to illegal move
			//writetoOutputFile(ILLEGAL_MOVE, opponent);
			return opponent;
		}

		//execute move
		unique_ptr<FightInfo> pt_fight;
		movePiece(board.getPiece(ptr_move->getFrom()), ptr_move->getTo(), pt_fight);

		//CHECK WINNER
		if (checkWinner(player)==1) {
			return player;
		}

		/*
		switch(checkWinner(player)){
		case 1:
			writetoOutputFile(PLAYER_FLAGS_EATEN, player);
			return 0;
		case 2:
			writetoOutputFile(PLAYER_NO_MOVING_PIECES, player);
			return 0;
		}
		*/

		opponent_player->notifyOnOpponentMove(*(ptr_move.get()));

		//notifying about last fight (if exists)
		noFightMoves++;
		if(pt_fight!=NULL) {
		//sumFights++;
			/*int X=pt_fight->getPosition().getX();
			int Y=pt_fight->getPosition().getY();
			RPS_FightInfo fight= RPS_FightInfo(X, Y, pt_fight->getPiece(player), pt_fight->getPiece(opponent), pt_fight->getWinner());
			cout << "X: " << X << " Y: " << Y << " player: " << pt_fight->getPiece(player) << " opponent: " << pt_fight->getPiece(opponent) << endl;

			cout<<"winner: "<<pt_fight.get()->getWinner()<<endl;
			*/
			current_player->notifyFightResult(*(pt_fight.get()));
			//current_player->notifyFightResult(fight);
			opponent_player->notifyFightResult(*(pt_fight.get()));
			//opponent_player->notifyFightResult(fight);

			noFightMoves=0;
		}

		//execute joker change
		ptr_jokerChange=std::move(current_player->getJokerChange());

		if(ptr_jokerChange!=NULL){
			if (!checkLegalJokerChange(*(ptr_jokerChange.get()), player)) {
				//writetoOutputFile(ILLEGAL_MOVE, opponent);
				return opponent;
			}
			changeJokerRep(*(ptr_jokerChange.get()));
		}

		//switching players
		current_player=(player==1) ? &player2 : &player1;
		opponent_player=(player==1) ? &player1 : &player2;
		player=(player==1) ? 2 : 1;
		opponent= (opponent==1)? 2:1;
	}

	//writetoOutputFile(TOO_MANY_MOVES_WITH_NO_FIGHT, 0);
	//cout << "sumFights: " << sumFights << endl;
	return 0;
}











/*
 * RPSGame.cpp
 *
 *  Created on: 1 ???? 2018
 *      Author: Vered
 */

#include "RPSGame.h"

Piece& RPSGame::getPieceRPSGame(int x, int y) {
	return board[x][y];
}

void RPSGame::setPieceRPSGame(const Piece &piece, int x, int y) {
	board[x][y]=piece;
}

/*
 * if rep is a legal representation for joker- return 1
 * else- return 0
 */
int isLegalJokerRep(char rep){
	if(rep=='R' || rep=='P' || rep=='S'|| rep=='B'){
		return 1;
	}
	return 0; //illegal joker representation
}

/*
 * called from "isLegalInputBoardFile"
 */
int inputBoardBadFormat(const RPSParserPosition &parsrPosition, int pieces_counter[]) {
	string available_char_for_joker="RPSB";
	char piece_char=parsrPosition.getPieceChar();

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

	//GOOD FORMAT!
	return 1;
}

int legalCoordinates(int column_coor, int row_coor) {
	if ((row_coor<0 || row_coor>N_ROWS-1) || (column_coor<0 || column_coor>M_COLUMNS-1)) { //not valid coordinate
		return 0;
	}
	//GOOD COORDINATES
	return 1;
}
/*
 * called from "isLegalInputBoardFile"
 */
int inputBoardCheckPiecesBound(int pieces_counter[]) {
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

/*
 * used in "executeGame", "isLegalInputBoardFile" & "insertPlayerPieces".
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

/*
 * called from "isLegalInputBoardFile", in case of not valid input!
 */
void inputBoardNotValidHandling(LosingDetails &losingdetails, int player, int current_winner,int line_number) {
	if (player==1) {
		losingdetails.setWinnerPlayer(2);
		losingdetails.setLineNumber1(line_number);
		losingdetails.setLosingCause(LosingCause::BAD_POSITIONING_FILE);
	}
	else if (player==2) {
		losingdetails.setWinnerPlayer(1);
		losingdetails.setLineNumber2(line_number);
		losingdetails.setLosingCause(LosingCause::BAD_POSITIONING_FILE);
	}

	if ((player==1 && current_winner==1) || (player==2 && current_winner==2)) { //meaning the other input file is also not valid
		losingdetails.setWinnerPlayer(0); //meaning a tie.
		losingdetails.setLosingCause(LosingCause::TIE_BAD_POSITIONING_FILES);
	}
}

/*
 * returns -1 in case of opening file failure.
 * other erro in the input file - returns 0.
 * upon success returns 1.
 */
int isLegalInputBoardFile(LosingDetails &losingdetails, const string& path, int player){
	string buffer;
	int column_coor=0;
	int row_coor=0;
	RPSParserPosition parsrPosition;
	int line_number=1;
	int is_input_valid=1;

	int piecesPositions[N_ROWS][M_COLUMNS]={0}; //we fill this matrix with 0's. change to 1 if a piece has been placed in this coordinates.
	int pieces_counter[6]={0}; //index: 0 - R, 1 - P, 2 - S, 3 - B, 4 - F, 5 - J

	ifstream posFile(path);
	if (!posFile.is_open()) { //could not open file! returns 0.
		cout << "ERROR: could not open file in path: " << path << endl;
		return -1;
	}

	int current_winner=losingdetails.getWinnerPlayer();
	while(std::getline(posFile, buffer)) {
		if(checkEmptyLine(buffer)) {
			continue;
		}
		parsrPosition=parseLinePosition(buffer);

		//check FORMAT
		if (!parsrPosition.getLegalFormat() || !inputBoardBadFormat(parsrPosition, pieces_counter)) {
			is_input_valid=0;
			cout << "ERROR: in boardFile of player " << player << ", not valid format" << endl;
			break;
		}

		//check format - is valid value for joker
		if (parsrPosition.getPieceChar()=='J') {
			if (!isLegalJokerRep(parsrPosition.getJokerPieceType())) {
				is_input_valid=0;
				cout << "ERROR: in boardFile of player " << player << ", not valid format(joker)" << endl;
				break;
			}
		}

		//get coordinates
		column_coor=parsrPosition.getColumnCoor();
		row_coor=parsrPosition.getRowCoor();

		//now checking coordinates: (of any piece)
		if (!legalCoordinates(column_coor, row_coor)) {
			is_input_valid=0;
			cout << "ERROR: in boardFile of player " << player << ", not valid COORDINATES" << endl;
			break;
		}

		//now checking no two pieces are positioned in the same place
		if (piecesPositions[row_coor][column_coor]==1) {
			is_input_valid=0;
			cout << "ERROR: in boardFile of player " << player << ", two or more PIECEs are positioned in the same location" << endl;
			break;
		}
		else {
			piecesPositions[row_coor][column_coor]=1;
		}

		//check if there are F_MAX flags, and other pieces counter does not exceed the bound
		if (!inputBoardCheckPiecesBound(pieces_counter)) {
			is_input_valid=0;
			cout << "ERROR: in boardFile of player " << player << ", a PIECE appears more than its number" << endl;
			break;
		}
		line_number++; //used in case of an error
	}

	//close the input file
	posFile.close();

	if (is_input_valid) {
		//is there the correct amount of FLAGS
		if (pieces_counter[4]!=F_MAX) {
			is_input_valid=0;
			cout << "ERROR: in boardFile of player " << player << ", Missing Flags - Flags are not positioned according to their number" << endl;
		}
	}

	//error handling while reading lines.
	if (!is_input_valid) {
		inputBoardNotValidHandling(losingdetails, player, current_winner, line_number);
		return 0;
	}

	//finally - valid input file.
	return 1;
}

/*
 * by the rules given in the homework instructions, we return 1 if piece1 is stronger, and 2 if piece 2 is stronger.
 * if both sides equal, we return 0.
 */
int fightPieces(const Piece &piece1, const Piece &piece2) {
	char type1=piece1.getType();
	char type2=piece2.getType();

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

void RPSGame::reduceGameFieldsUponMoving(const Piece &piece) {
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

void RPSGame::movePiece(Piece &piece, int column_coor, int row_coor) {
	int winning_piece;
	if(board[row_coor][column_coor].getType()==0){
		board[row_coor][column_coor]=piece;
		piece=Piece();
		return;
	}
	winning_piece=fightPieces(piece, board[row_coor][column_coor]);

	if (winning_piece==0) {
		//take care of field of moving pieces
		reduceGameFieldsUponMoving(piece);
		reduceGameFieldsUponMoving(board[row_coor][column_coor]);

		board[row_coor][column_coor]=Piece(0,0,0);

			}
	else if (winning_piece==1) {
		reduceGameFieldsUponMoving(board[row_coor][column_coor]);
		board[row_coor][column_coor]=piece;


	}
	else { //if winning_piece==2, no need for change the board
		reduceGameFieldsUponMoving(piece);
	}

	//if the winning piece is a bomb, we need to erase it now.
	if (board[row_coor][column_coor].getType()=='B') {
		//if the bomb is originally a joker - need to update game fields - CHECK IF REALLY NEED TO DO IT
		reduceGameFieldsUponMoving(board[row_coor][column_coor]);
		board[row_coor][column_coor]=Piece(0,0,0);
	}

	//set piece to initial values, like marking "null square" on the game board
	piece=Piece();
}

void RPSGame::increaseGameFieldsUponMoving(const Piece &tmpPiece) {
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

int RPSGame::insertPlayerPieces(const string& path, int player) {
	ifstream posFile(path);
	if (!posFile.is_open()) { //could not open file! returns 0.
		cout << "ERROR: could not open file in path: " << path << " due to system failure\n." << endl;
		return -1;
	}

	string buffer;
	Piece tmpPiece;
	RPSParserPosition parsrPosition;


	while(std::getline(posFile, buffer)) {
		if(checkEmptyLine(buffer)){
			continue;
		}
		parsrPosition=parseLinePosition(buffer);

		//INSERT VALUES TO LINE_PARAMETERS

		if (parsrPosition.getPieceChar()=='J') { //this is a joker!
			tmpPiece=Piece(parsrPosition.getJokerPieceType(), 1, player);
		}
		else { //regular piece
			tmpPiece=Piece(parsrPosition.getPieceChar(), 0, player);
		}

		//update the game fields (is tmpPiece a moving piece?, a flag?)
		increaseGameFieldsUponMoving(tmpPiece);


		//insert piece to the board - validity of moves was already checked!
		movePiece(tmpPiece, parsrPosition.getColumnCoor(), parsrPosition.getRowCoor());
	}

	//end of inserting. all was well.
	return 1;
}

int RPSGame::initRPSGame(LosingDetails &losingdetails, const string& path1, const string& path2){
	//check first if inputs are valid - isLegalInputBoardFile returns: -1 - error in opening file, 0 - error, 1 - all was well.
	int input_player1=isLegalInputBoardFile(losingdetails, path1, 1);
	int input_player2=isLegalInputBoardFile(losingdetails, path2, 2);

	if (input_player1==-1 || input_player2==-1) {
		return -1; //meaning can't open one of the files - a message already been printed. exiting game.
	}

	if (!(input_player1==1 && input_player2==1)) { //means there's a loser.
		return 0;
	}

	//two valid files, now place Pieces on the board
	if (insertPlayerPieces(path1, 1)==-1 || insertPlayerPieces(path2, 2)==-1) {
		return -1; //fail in opening again one of the files, due to system failure and not because of the files. message been printed.
	}

	int is_there_winner=checkWinnerAfterPosition(losingdetails); //change the losingdetails if needed.

	if (is_there_winner) {
		return 0;
	}

	return 1; //continue playing - go to moves files.
}

int RPSGame::checkWinnerAfterPosition(LosingDetails &losingdetails){
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

/*
 * called from "writeToOutputFile" to determine which is the reason for the win, based on info from losingdetails.
 */
string writeToOutputFileMakeReason(const LosingDetails &losingdetails) {
	string reason;
	stringstream reason_stream;
	int winner=losingdetails.getWinnerPlayer();
	LosingCause cause=losingdetails.getLosingCause();

	if (cause==LosingCause::All_FLAGS_CAPTURED) {
		reason="All flags of the opponent are captured";
	}
	else if (cause==LosingCause::MOVING_PIECES_GONE) {
		reason="All moving PIECEs of the opponent are eaten";
	}
	else if (cause==LosingCause::TIE_MOVE_FILES_DONE) {
		reason="A tie - both Moves input files done without a winner";
	}
	else if (cause==LosingCause::TIE_FLAGS_EATEN_POSITION_FILES) {
		reason="A tie - all flags are eaten by both players in the position files";
	}
	else if (cause==LosingCause::BAD_POSITIONING_FILE) {
		if (winner==1) {
			reason_stream << "Bad Positioning input file for player 2 - line " << losingdetails.getLineNumber2();
			reason = reason_stream.str();
		}
		else if (winner==2) {
			reason_stream << "Bad Positioning input file for player 1 - line " << losingdetails.getLineNumber1();
			reason = reason_stream.str();
		}
	}
	else if (cause==LosingCause::TIE_BAD_POSITIONING_FILES) {
		reason_stream << "Bad Positioning input file for both players - player 1: line " << losingdetails.getLineNumber1() <<
				", player 2: line " << losingdetails.getLineNumber2();
		reason = reason_stream.str();
	}
	else if (cause==LosingCause::BAD_MOVES_FILE) {
		if (winner==1) {
			reason_stream << "Bad Moves input file for player 2 - line " << losingdetails.getLineNumber2();
			reason = reason_stream.str();
		}
		else if (winner==2) {
			reason_stream << "Bad Moves input file for player 1 - line " << losingdetails.getLineNumber1();
			reason = reason_stream.str();
		}
	}
	return reason;
}

void writeToOutputFile(const string &output_path, RPSGame &game, LosingDetails &losingdetails) {
	string reason;
	Piece piece;
	char pieceType;

	ofstream outFile(output_path);
	if (!outFile.is_open()) { //could not open file! returns 0.
		cout << "ERROR: could not open output file" << endl;
		return;
	}

	int winner=losingdetails.getWinnerPlayer();
	reason=writeToOutputFileMakeReason(losingdetails);
	outFile << "Winner: " << winner << endl;
	outFile << "Reason: " << reason << endl;
	outFile << endl;

	//now print the board
	for (int i=0; i<N_ROWS; i++) {
		for (int j=0; j<M_COLUMNS; j++) {
			piece=game.getPieceRPSGame(i,j);
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
}

int isValidMoveOneSquare(int from_row, int from_column, int to_row, int to_column) {
	if (abs(from_row-to_row)==1 && from_column==to_column) {
		return 1;
	}
	else if (abs(from_column-to_column)==1 && from_row==to_row) {
		return 1;
	}
	return 0;
}

int RPSGame::executeGame(const string& path1,const string& path2, LosingDetails &result){
	int player=1;
	int opponent=2;
	string line;
	RPSParserMove parsedLine;
	int cnt_lines_p1=0;
	int cnt_lines_p2=0;
	int file1_done=0;
	int file2_done=0;
	int tmp;

	ifstream movesFile1(path1);
	if (!movesFile1.is_open()) { //can't open moves file of player 1.
		cout<< "Error opening Moves File of player1."<< endl;
		return 0;
	}

	ifstream movesFile2(path2);
	if (!movesFile2.is_open()) { //can't open moves file of player 1.
		movesFile1.close();
		cout<< "Error opening Moves File of player2."<< endl;
		return 0;
	}

	while(!(file1_done && file2_done)){

		if(playerHasFlag(player)==0){
			result.setWinnerPlayer(opponent);
			result.setLosingCause(LosingCause::All_FLAGS_CAPTURED);
			break;
		}
		if(playerHasMovingPieces(player)==0){
			result.setWinnerPlayer(opponent);
			result.setLosingCause(LosingCause::MOVING_PIECES_GONE);
			break;
		}

		if(player==1 && !file1_done){
			if(std::getline(movesFile1, line)){
				if(checkEmptyLine(line)){
					continue;
				}
				cnt_lines_p1++;
				parsedLine= parseLine(line);
				if(isValidMoveLine(player, parsedLine)){
					executeMove(parsedLine, player);
				}
				else{
					result.setWinnerPlayer(2);
					result.setLosingCause(LosingCause::BAD_MOVES_FILE);
					result.setLineNumber1(cnt_lines_p1);
					break;
				}
			}
			else{
				if(!movesFile1.eof()){
					movesFile1.close();
					movesFile2.close();
					cout<<"Error reading line from MOVES file of player 1"<<endl;
					return 0;
				}
				file1_done=1;
			}
		}
		else if(player==2 && !file2_done){
			if(std::getline(movesFile2, line)){
				if(checkEmptyLine(line)){
					continue;
				}
				cnt_lines_p2++;
				parsedLine= parseLine(line);
				if(isValidMoveLine(player, parsedLine)){
					executeMove(parsedLine, player);
				}
				else{
					result.setWinnerPlayer(1);
					result.setLosingCause(LosingCause::BAD_MOVES_FILE);
					result.setLineNumber2(cnt_lines_p2);
					break;
				}
			}
			else{
				if(!movesFile2.eof()){
					movesFile1.close();
					movesFile2.close();
					cout<<"Error reading line from MOVES file of player 2"<<endl;
					return 0;
				}
				file2_done=1;
			}
		}
		tmp=player;
		player=opponent;
		opponent=tmp;
	}
	if(file1_done && file2_done){
		result.setWinnerPlayer(0);
		result.setLosingCause(LosingCause::TIE_MOVE_FILES_DONE);
	}

	movesFile1.close();
	movesFile2.close();

	return 1;
}

int movesFilesExist(const string& path1,const string& path2){
	ifstream movesFile1(path1);
	if (!movesFile1.is_open()) { //can't open moves file of player 1.
		cout<< "Error opening Moves File of player1."<< endl;
		return 0;
	}

	ifstream movesFile2(path2);
	if (!movesFile2.is_open()) { //can't open moves file of player 1.
		movesFile1.close();
		cout<< "Error opening Moves File of player2."<< endl;
		return 0;
	}
	return 1;
}

int RPSGame::playerHasMovingPieces(int player){
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

int RPSGame::playerHasFlag(int player){
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

void RPSGame::executeMove(RPSParserMove &parsedLine, int player){
	int fromRow, fromCol, toRow, toCol;
	int jokerCol, jokerRow;
	char jokerRep;

	fromRow= parsedLine.getFromRow();
	fromCol= parsedLine.getFromCol();
	toRow= parsedLine.getToRow();
	toCol= parsedLine.getToCol();

	//perform the piece move
	movePiece(board[fromRow][fromCol], toCol, toRow);
	//change joker if needed
	if(parsedLine.isJokerChanged()){
		jokerRow= parsedLine.getJokerRow();
		jokerCol= parsedLine.getJokerCol();
		jokerRep= parsedLine.getJokerNewRep();
		if(board[jokerRow][jokerCol].getIsJoker()&& board[jokerRow][jokerCol].getOwnerPlayer()==player){
			changeJokerRep(jokerRow, jokerCol, jokerRep);

		}
	}
}

int RPSGame::isValidMoveLine(int player, RPSParserMove &parserdLine){
	int from_row=parserdLine.getFromRow();
	int from_col=parserdLine.getFromCol();
	int to_row=parserdLine.getToRow();
	int to_col= parserdLine.getToCol();
	int joker_row, joker_col;
	char joker_rep;

	if(!parserdLine.isLegalFormat()){
		cout<< "Illegal move of player "<<player<<": bad format."<<endl;
		return 0;
	}
	if(!legalCoordinates(parserdLine.getFromCol(), parserdLine.getFromRow())){
		cout<< "Illegal move of player "<<player<<": illegal origin position for piece."<<endl;
		return 0;
	}
	if(!legalCoordinates(parserdLine.getToCol(), parserdLine.getToRow())){
		cout<< "Illegal move of player "<<player<<": illegal destination position for piece."<<endl;
		return 0;
	}
	if(board[from_row][from_col].getOwnerPlayer()!=player){
		cout<< "Illegal move of player "<<player<<": position doesn't contain a piece of the player."<<endl;
		return 0;
	}
	if (!board[from_row][from_col].isMovingPiece()){
		cout<< "Illegal move of player "<<player<<": the piece in the position can't move."<<endl;
		return 0;
	}
	if(board[to_row][to_col].getOwnerPlayer()==player){
		cout<< "Illegal move of player "<<player<<": destination position is already taken by player "<<player<<"."<<endl;
		return 0;
	}

	if(!isValidMoveOneSquare(from_row, from_col, to_row, to_col)){
		cout<< "Illegal move of player "<<player<<": piece can move either one column or one row."<<endl;
		return 0;
	}
	if(!parserdLine.isJokerChanged()){
		return 1; //no need to check information for joker change
	}

	joker_row=parserdLine.getJokerRow();
	joker_col= parserdLine.getJokerCol();
	joker_rep=parserdLine.getJokerNewRep();

	if(!legalCoordinates(joker_col, joker_row)){
		cout<< "Illegal move of player "<<player<<": illegal position for joker."<<endl;
		return 0;
		//add info- joker position is wrong
	}
	//if the moved piece is a joker and it's moved to the given joker's position- the position (will) contain a joker owned by the player
	if(board[from_row][from_col].getIsJoker() && to_row==joker_row && to_col==joker_col){

	}
	//if the joker is in the place of the moved piece- after the move the position won't contain a joker
	else if(from_row==joker_row && from_col==joker_col){
		cout<< "Illegal move of player "<<player<<": position doesn't contain a joker owned by player "<<player<<"."<<endl;
				return 0;
	}
	//if the position is unrelated to the moved piece
	else if(!board[joker_row][joker_col].getIsJoker() || board[joker_row][joker_col].getOwnerPlayer()!=player){
		cout<< "Illegal move of player "<<player<<": position doesn't contain a joker owned by player "<<player<<"."<<endl;
		return 0;
	}
	if(!isLegalJokerRep(joker_rep)){
		cout<< "Illegal move of player "<<player<<": not a legal REP for joker."<<endl;
		return 0;
	}
	return 1;
}

void RPSGame::changeJokerRep(int row, int col, char rep){
	Piece &joker= board[row][col];
	int player= joker.getOwnerPlayer();
	int ismovingRep= rep=='B'? 0:1; // if the new rep is of a moving piece=1; else 0;
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
	joker.setType(rep);
}

void RPSGame::printBoard() {
	string reason;
	Piece piece;
	char pieceType;

	//now print the board
	for (int i=0; i<N_ROWS; i++) {
		for (int j=0; j<M_COLUMNS; j++) {
			piece= board[i][j];
			pieceType=piece.getType();
			if (piece.getOwnerPlayer()==0) { //"null square"
				cout << "_";
			}
			//o/w it belongs to some player!
			else if (piece.getOwnerPlayer()==1) {
			/*	if (piece.getIsJoker()) {
					cout << "J";
				}*/
			//	else {
					cout << pieceType;
				//}
			}
			else if (piece.getOwnerPlayer()==2) {
				//if (piece.getIsJoker()) {
					//cout << "j";
					//continue;
			//	}
				if (pieceType=='R') {cout << "r";}
				if (pieceType=='P') {cout << "p";}
				if (pieceType=='S') {cout << "s";}
				if (pieceType=='B') {cout << "b";}
				if (pieceType=='F') {cout << "f";}
			}
		}
		cout << endl;
	}
}


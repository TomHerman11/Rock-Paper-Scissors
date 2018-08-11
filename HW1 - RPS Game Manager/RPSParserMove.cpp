/*
 * RPSParser.cpp
 *
 *  Created on: 4 ???? 2018
 *      Author: Vered
 */

#include "RPSParserMove.h"


RPSParserMove::RPSParserMove(){
	legal_format=0;
	from_row=0;
	from_col=0;
	to_row=0;
	to_col=0;
	changeJoker=0;
	joker_row=0;
	joker_col=0;
	joker_new_rep=0;
}
RPSParserMove::RPSParserMove(int fromRow, int fromCol, int toRow, int toCol){

	legal_format=1;
	from_row=fromRow;
	from_col=fromCol;
	to_row= toRow;
	to_col= toCol;
	changeJoker=0;
	joker_row=-1;
	joker_col=-1;
	joker_new_rep=0;

}

void RPSParserMove::addJokerChangeInfo(int jokerRow, int jokerCol, char jokerNewRep){
	changeJoker=1;
	joker_row= jokerRow;
	joker_col= jokerCol;
	joker_new_rep= jokerNewRep;
}

RPSParserMove parseLine(const string& line){
	RPSParserMove parsedMove;
	string detailsArr[9];
	int param_cnt=9;
	int fromRow, fromCol, toRow, toCol;
	int jokerRow, jokerCol;
	string tmp;

	std::istringstream st(line);

	//extract move parameters from the line. an extra word is extracted to check the format
	for(int i=0; i<9; i++){
		st>>detailsArr[i];
	}

	//check the amount of the parameters
	for(int i=0; i<9; i++){
		if(detailsArr[i].empty()){
			param_cnt=i;
			break;
		}
	}
	//legal format includes 4 or 8 parameters
	if(!(param_cnt==4 || param_cnt==8)){
		return RPSParserMove(); //return a parser with legal_format field set to false (0).
	}


	//check that the four first parameters are numbers. if not- the format is wrong
	for(int i=0; i<4; i++){
		if(detailsArr[i].find_first_not_of("0123456789")!=string::npos){
			return RPSParserMove();
		}
	}
	std::istringstream st2(line);

	//the parameters can be converted to int
	st2>>fromCol>>fromRow>> toCol>> toRow;
	parsedMove= RPSParserMove(fromRow-1, fromCol-1, toRow-1, toCol-1);

	//check if there is a joker change in the move. if not, we finished parsing the move
	if(param_cnt==4){
		return parsedMove;
	}

	//if there is a joker, next parameter (5th) should be "J:"
	if(detailsArr[4]!="J:"){
		return RPSParserMove();
	}
	//check that the 6th and 7th parameters are integers
	if(detailsArr[5].find_first_not_of("0123456789")!=string::npos ||
			detailsArr[6].find_first_not_of("0123456789")!=string::npos){
		return RPSParserMove();
	}

	//check if the last parameter isn't a char
	if(detailsArr[7].length()!=1){
		return RPSParserMove();
	}

	st2>>tmp>>jokerCol>>jokerRow;
	parsedMove.addJokerChangeInfo(jokerRow-1, jokerCol-1, detailsArr[7][0]);

	return parsedMove;



}

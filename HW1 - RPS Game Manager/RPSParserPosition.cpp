/*
 * RPSParser.cpp
 *
 *  Created on: 4 ???? 2018
 *      Author: Vered
 */

#include "RPSParserPosition.h"

RPSParserPosition::RPSParserPosition(int legalformat) { //c'tor for BAD FORMAR!
	legalFormat=legalformat;
	pieceChar=0;
	columnCoor=0;
	rowCoor=0;
	jokerPieceChar=0;
}

RPSParserPosition::RPSParserPosition(int legalformat, char piecechar, int columncoor, int rowcoor) {
	legalFormat=legalformat;
	pieceChar=piecechar;
	columnCoor=columncoor;
	rowCoor=rowcoor;
	jokerPieceChar=0;
}

void RPSParserPosition::setJokerPieceType(char jokerpiecetype) {
	jokerPieceChar=jokerpiecetype;
}

RPSParserPosition parseLinePosition(const string& line) {
	RPSParserPosition parserposition;
	string line_details[5];
	int is_it_joker=0;
	char pieceChar, jokerPieceChar;
	int columnCoor, rowCoor;
	int amount_parameters=0;

	std::istringstream st(line);

	//extract position parameters from the line. an extra word is extracted to check the format
	for(int i=0; i<5; i++){
		st>>line_details[i];
	}

	//check the amount of the parameters - we need 3 or 4
	for(int i=0; i<5; i++){
		if(line_details[i].empty()){
			amount_parameters=i;
			break;
		}
	}

	//check the amount of the parameters
	if (amount_parameters!=3 && amount_parameters!=4) {
		return RPSParserPosition(); //illegal format;
	}

	//check the first parameter is of length 1
	if (line_details[0].length()!=1) {
		return RPSParserPosition();
	}

	//check if the first parameter is 'J', there must be a fourth parameter
	if (line_details[0]=="J") {
		if (line_details[3].length()!=1) {
			return RPSParserPosition();
		}
		else {
			is_it_joker=1;
		}
	}

	//check that the second and third parameters are numbers. if not - the format is wrong
	if(line_details[1].find_first_not_of("0123456789")!=string::npos ||
			line_details[2].find_first_not_of("0123456789")!=string::npos)
	{
		return RPSParserPosition();
	}

	//for making the Parser
	std::istringstream st2(line);
	st2>>pieceChar>>columnCoor>>rowCoor;

	parserposition=RPSParserPosition(1, pieceChar, columnCoor-1, rowCoor-1);

	//is it joker?
	if (is_it_joker) {
		st2>>jokerPieceChar;
		parserposition.setJokerPieceType(jokerPieceChar);
	}
	else{
		if(amount_parameters>3){
			return RPSParserPosition();
		}
	}

	return parserposition;
}

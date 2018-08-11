/*
 * RPS_ParserPosition.cpp
 *
 *  Created on: 6 ???? 2018
 *      Author: Vered
 */

#include "RPS_ParserPosition.h"


RPS_ParserPosition::RPS_ParserPosition(int legalformat) { //c'tor for BAD FORMAT!
	legalFormat=legalformat;
	pieceChar=0;
	columnCoor=0;
	rowCoor=0;
	jokerPieceChar=0;
}

RPS_ParserPosition::RPS_ParserPosition(int legalformat, char piecechar, int columncoor, int rowcoor) {
	legalFormat=legalformat;
	pieceChar=piecechar;
	columnCoor=columncoor;
	rowCoor=rowcoor;
	jokerPieceChar=0;
}

void RPS_ParserPosition::setJokerPieceType(char jokerpiecetype) {
	jokerPieceChar=jokerpiecetype;
}

RPS_ParserPosition parseLinePosition(const string& line) {
	RPS_ParserPosition parserposition;
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
		return RPS_ParserPosition(); //illegal format;
	}

	//check the first parameter is of length 1
	if (line_details[0].length()!=1) {
		return RPS_ParserPosition();
	}

	//check if the first parameter is 'J', there must be a fourth parameter
	if (line_details[0]=="J") {
		if (line_details[3].length()!=1) {
			return RPS_ParserPosition();
		}
		else {
			is_it_joker=1;
		}
	}

	//check that the second and third parameters are numbers. if not - the format is wrong
	if(line_details[1].find_first_not_of("0123456789")!=string::npos ||
			line_details[2].find_first_not_of("0123456789")!=string::npos)
	{
		return RPS_ParserPosition();
	}

	//for making the Parser
	std::istringstream st2(line);
	st2>>pieceChar>>columnCoor>>rowCoor;

	parserposition=RPS_ParserPosition(1, pieceChar, columnCoor, rowCoor);

	//is it joker?
	if (is_it_joker) {
		st2>>jokerPieceChar;
		parserposition.setJokerPieceType(jokerPieceChar);
	}
	else{
		if(amount_parameters>3){
			return RPS_ParserPosition();
		}
		else{
			parserposition.setJokerPieceType('#');

		}
	}

	return parserposition;
}

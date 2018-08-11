/*
 * RPSParser.h
 *
 *  Created on: 4 ???? 2018
 *      Author: Vered
 */

#ifndef RPSPARSERMOVE_H_
#define RPSPARSERMOVE_H_

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
using namespace std;

class RPSParserMove{
	int legal_format;
	int from_row;
	int from_col;
	int to_row;
	int to_col;
	int changeJoker;
	int joker_row;
	int joker_col;
	char joker_new_rep;

public:
	RPSParserMove();
	RPSParserMove(int fromRow, int fromCol, int toRow, int toCol);
	void addJokerChangeInfo(int joker_row, int joker_col, char joker_new_rep);
	void setLegalFormat(int isLegal) {legal_format=isLegal;}
	int getFromRow()const {return from_row;}
	int getFromCol()const {return from_col;}
	int getToRow()const {return to_row;}
	int getToCol()const {return to_col;}
	int isJokerChanged()const {return changeJoker;}
	int getJokerRow()const {return joker_row;}
	int getJokerCol()const {return joker_col;}
	char getJokerNewRep()const {return joker_new_rep;}
	int isLegalFormat()const {return legal_format;}

};

RPSParserMove parseLine(const string& line);


#endif /* RPSPARSERMOVE_H_ */

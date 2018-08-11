/*
 * RPSParser.h
 *
 *  Created on: 4 ???? 2018
 *      Author: Vered
 */

#ifndef RPSPARSERPOSITION_H_
#define RPSPARSERPOSITION_H_

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
using namespace std;

#define LINE_LENGTH 4096

class RPSParserPosition{
	int legalFormat;
	char pieceChar;
	int columnCoor;
	int rowCoor;
	char jokerPieceChar;

public:
	RPSParserPosition(): legalFormat(0), pieceChar(0), columnCoor(0), rowCoor(0), jokerPieceChar(0) {}
	RPSParserPosition(int legalformat);
	RPSParserPosition(int legalformat, char piecechar, int columncoor, int rowcoor);

	int getLegalFormat()const {return legalFormat;}
	char getPieceChar()const {return pieceChar;}
	int getColumnCoor()const {return columnCoor;}
	int getRowCoor()const {return rowCoor;}
	char getJokerPieceType()const {return jokerPieceChar;}
	void setJokerPieceType(char jokerpiecetype);
};

/*
 * this functions parses the line into the parameters needed in each line in the board input file.
 * if the line is not in the form of a legal board line, meaning:
 * <CHAR> <INT> <INT> OR <CHAR(JOKER)> <INT> <INT> <CHAR>
 * legalFormat is 0. o/w 1.
 */
RPSParserPosition parseLinePosition(const string& line);

#endif /* RPSPARSERPOSITION_H_ */

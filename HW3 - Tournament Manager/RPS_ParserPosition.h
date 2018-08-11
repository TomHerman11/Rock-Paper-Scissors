/*
 * RPS_ParserPosition.h
 *
 *  Created on: 6 ???? 2018
 *      Author: Vered
 */

#ifndef RPS_PARSERPOSITION_H_
#define RPS_PARSERPOSITION_H_


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
using namespace std;

#define LINE_LENGTH 4096

class RPS_ParserPosition{
	int legalFormat;
	char pieceChar;
	int columnCoor;
	int rowCoor;
	char jokerPieceChar;

public:
	RPS_ParserPosition(): legalFormat(0), pieceChar(0), columnCoor(0), rowCoor(0), jokerPieceChar(0) {}
	RPS_ParserPosition(int legalformat);
	RPS_ParserPosition(int legalformat, char piecechar, int columncoor, int rowcoor);

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
RPS_ParserPosition parseLinePosition(const string& line);




#endif /* RPS_PARSERPOSITION_H_ */

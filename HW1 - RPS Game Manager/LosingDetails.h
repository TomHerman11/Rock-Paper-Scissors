/*
 * LosingDetails.h
 *
 *  Created on: Apr 4, 2018
 *      Author: tomhe
 */

#ifndef LOSINGDETAILS_H_
#define LOSINGDETAILS_H_

enum class LosingCause {
	NO_CAUSE,
	All_FLAGS_CAPTURED,
	MOVING_PIECES_GONE,
	BAD_MOVES_FILE,
	BAD_POSITIONING_FILE,
	TIE_BAD_POSITIONING_FILES,
	TIE_FLAGS_EATEN_POSITION_FILES,
	TIE_MOVE_FILES_DONE,
};

class LosingDetails {
	int winnerPlayer;
	int lineNumber1;
	int lineNumber2;
	LosingCause cause;

public:
	LosingDetails(): winnerPlayer(-1), lineNumber1(-1), lineNumber2(-1) {cause=LosingCause::NO_CAUSE;}

	void setWinnerPlayer(int winnerplayer) {winnerPlayer=winnerplayer;}
	void setLineNumber1(int linenumber1) {lineNumber1=linenumber1;}
	void setLineNumber2(int linenumber2) {lineNumber2=linenumber2;}
	void setLosingCause(LosingCause losingcause) {cause=losingcause;}
	int getWinnerPlayer()const {return winnerPlayer;}
	int getLineNumber1()const {return lineNumber1;}
	int getLineNumber2()const {return lineNumber2;}
	LosingCause getLosingCause()const {return cause;}
};

#endif /* LOSINGDETAILS_H_ */

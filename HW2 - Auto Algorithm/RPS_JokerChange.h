/*
 * RPS_JokerChange.h
 *
 *  Created on: Apr 29, 2018
 *      Author: tomhe
 */

#ifndef RPS_JOKERCHANGE_H_
#define RPS_JOKERCHANGE_H_

#include "JokerChange.h"
#include "RPS_Point.h"


class RPS_JokerChange: public JokerChange {
	const RPS_Point point;
	char type;

public:
	RPS_JokerChange(int col, int row, char type): point(col,row), type(type) {}
	virtual const Point& getJokerChangePosition()const override {return point;}
	virtual char getJokerNewRep()const override {return type;} // R, P, S or B (but NOT J and NOT F)
	//virtual ~RPS_JokerChange() {}
};

#endif /* RPS_JOKERCHANGE_H_ */

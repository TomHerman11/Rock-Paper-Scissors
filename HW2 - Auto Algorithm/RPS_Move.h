/*
 * RPS_Move.h
 *
 *  Created on: Apr 29, 2018
 *      Author: tomhe
 */

#ifndef RPS_MOVE_H_
#define RPS_MOVE_H_

#include "Move.h"
#include "RPS_Point.h"

class RPS_Move: public Move {
	RPS_Point from;
	RPS_Point to;

public:
	RPS_Move(RPS_Point from_point, RPS_Point to_point) {from=from_point; to=to_point;}
	RPS_Move(int from_col, int from_row, int to_col, int to_row):
					from(from_col, from_row), to(to_col, to_row) {}

	virtual const Point& getFrom()const override {return from;}
	virtual const Point& getTo()const override {return to;}
	//virtual ~RPS_Move() {}

};

#endif /* RPS_MOVE_H_ */

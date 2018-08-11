/*
 * RPS_Point.h
 *
 *  Created on: Apr 29, 2018
 *      Author: tomhe
 */

#ifndef RPS_POINT_H_
#define RPS_POINT_H_

#include "Point.h"

class RPS_Point: public Point {
	int col; //=X
	int row; //=Y
public:
	RPS_Point(): col(0), row(0) {}
	RPS_Point(int col, int row): col(col), row(row) {}
	virtual int getX()const override {return col;}
	virtual int getY()const override {return row;}
	//virtual ~RPS_Point() {}
};


#endif /* RPS_POINT_H_ */

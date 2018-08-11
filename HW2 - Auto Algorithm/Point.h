#ifndef __POINT_H_
#define __POINT_H_

//--------------------------
// Point Interface
//--------------------------
// abstract class Point should be the base class for your "Point" implementation
// @author Amir Kirsh, TAU - Advanced Topics in Programming - 2018 Semester B

class Point {
public:
	virtual int getX() const = 0;
	virtual int getY() const = 0;
	virtual ~Point() {}
};

#endif
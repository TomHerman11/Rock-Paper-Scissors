#ifndef __JOKER_CHANGE_H_
#define __JOKER_CHANGE_H_

//--------------------------
// JokerChange Interface
//--------------------------
// abstract class JokerChange should be the base class for your "JokerChange" implementation
// returned from PlayerAlgorithm to "Game" in the flow:
// "Game" -> PlayerAlgorithm : virtual unique_ptr<JokerChange> getJokerChange(); // nullptr if no change is requested
// Note that "Game" is in parantheses because this is your class and you can choose its name

// @author Amir Kirsh, TAU - Advanced Topics in Programming - 2018 Semester B

//--------------------------
// forward declaration
class Point;
//--------------------------

class JokerChange {
public:
	virtual const Point& getJokerChangePosition() const = 0;
	virtual char getJokerNewRep() const = 0; // R, P, S or B (but NOT J and NOT F)
	virtual ~JokerChange() {}
};

#endif
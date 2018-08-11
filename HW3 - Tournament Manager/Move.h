#ifndef __MOVE_H_
#define __MOVE_H_

//--------------------------
// Move Interface
//--------------------------
// abstract class Move should be the base class for your "Move" implementation
// This interface is in use in two flows:
// (a) returned from PlayerAlgorithm to "Game" in the flow:
// "Game" -> PlayerAlgorithm : virtual unique_ptr<Move> getMove();
// (b) sent to PlayerAlgorithm by "Game" in the flow:
// "Game" -> PlayerAlgorithm : notifyOnOpponentMove(const Move& move); // called only on opponent’s move
// Note that "Game" is in parantheses because this is your class and you can choose its name

// @author Amir Kirsh, TAU - Advanced Topics in Programming - 2018 Semester B

//--------------------------
// forward declaration
class Point;
//--------------------------

class Move {
public:
	virtual const Point& getFrom() const = 0;
	virtual const Point& getTo() const = 0;
	virtual ~Move() {}
};

#endif
#ifndef __PIECE_POSITION_H_
#define __PIECE_POSITION_H_

//--------------------------
// PiecePosition Interface
//--------------------------
// abstract class PiecePosition should be the base class for your "PiecePosition" implementation
// This interface is in use in the flow:
// "Game" -> PlayerAlgorithm : getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill);
// PlayerAlgorithm is responsible of filling the vector sent to it with the initial positions of all his pieces 
// Note that "Game" is in parantheses because this is your class and you can choose its name

// @author Amir Kirsh, TAU - Advanced Topics in Programming - 2018 Semester B

//--------------------------
// forward declaration
class Point;
//--------------------------

class PiecePosition {
public:
	virtual const Point& getPosition() const = 0;
	virtual char getPiece() const = 0; // R, P, S, B, J or F
	virtual char getJokerRep() const = 0; // ONLY for Joker: R, P, S or B -- non-Joker may return ‘#’
	virtual ~PiecePosition() {}
};

#endif
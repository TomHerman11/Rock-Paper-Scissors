#ifndef __BOARD_H_
#define __BOARD_H_

//--------------------------
// Board Interface
//--------------------------
// abstract class Board should be the base class for your "Board" implementation
// sent from "Game" to PlayerAlgorithm in the flow:
// "Game" -> PlayerAlgorithm : notifyOnInitialBoard(const Board& b, const std::vector<unique_ptr<FightInfo>>& fights);
// Note that "Game" is in parantheses because this is your class and you can choose its name

// @author Amir Kirsh, TAU - Advanced Topics in Programming - 2018 Semester B

//--------------------------
// forward declaration
class Point;
//--------------------------

class Board {
public:
	virtual int getPlayer(const Point& pos) const = 0; // 1 for player 1’s piece, 2 for 2, 0 if empty
	virtual ~Board() {}
};

#endif
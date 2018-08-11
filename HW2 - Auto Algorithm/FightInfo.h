#ifndef __FIGHT_INFO_H_
#define __FIGHT_INFO_H_

//--------------------------
// FightInfo Interface
//--------------------------
// abstract class FightInfo should be the base class for your "FightInfo" implementation
// sent from "Game" to PlayerAlgorithm in 2 flows:
// "Game" -> PlayerAlgorithm : notifyOnInitialBoard(const Board& b, const std::vector<unique_ptr<FightInfo>>& fights);
// "Game" -> PlayerAlgorithm : notifyFightResult(const FightInfo& fightInfo);
// Note that "Game" is in parantheses because this is your class and you can choose its name

// @author Amir Kirsh, TAU - Advanced Topics in Programming - 2018 Semester B

//--------------------------
// forward declaration
class Point;
//--------------------------

class FightInfo {
public:
	virtual const Point& getPosition() const = 0;
	virtual char getPiece(int player) const = 0; // R, P, S, B or F (but NOT J)
	virtual int getWinner() const = 0; // 0 - both lost / tie, 1 - player 1 won, 2 - player 2 won
	virtual ~FightInfo() {}
};

#endif
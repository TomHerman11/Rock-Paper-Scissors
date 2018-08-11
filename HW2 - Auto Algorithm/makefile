COMP = g++-5.3.0
OBJS = main.o AutoPlayerAlgorithm.o FilePlayerAlgorithm.o RPS_Board.o \
RPS_FightInfo.o RPS_Manager.o RPS_ParserMove.o RPS_ParserPosition.o \
RPS_Piece.o

EXEC = ex2
CPP_COMP_FLAG = -std=c++14 -Wall -Wextra -Werror -pedantic-errors -DNDEBUG

#$(EXEC): $(OBJS)
#	$(COMP) $(OBJS) $(CPP_COMP_FLAG) -o $@

$(EXEC): $(OBJS)
	$(COMP) $(OBJS) -o $@

main.o: main.cpp RPS_Manager.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
AutoPlayerAlgorithm.o: AutoPlayerAlgorithm.cpp AutoPlayerAlgorithm.h PlayerAlgorithm.h RPS_Move.h RPS_JokerChange.h RPS_PiecePosition.h RPS_ParserMove.h RPS_ParserPosition.h RPS_Board.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
FilePlayerAlgorithm.o: FilePlayerAlgorithm.cpp FilePlayerAlgorithm.h PlayerAlgorithm.h RPS_Move.h RPS_JokerChange.h RPS_PiecePosition.h RPS_ParserMove.h RPS_ParserPosition.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPS_Board.o: RPS_Board.cpp RPS_Board.h Board.h RPS_Piece.h Point.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPS_FightInfo.o: RPS_FightInfo.cpp RPS_FightInfo.h RPS_Point.h FightInfo.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPS_Manager.o: RPS_Manager.cpp RPS_Manager.h PlayerAlgorithm.h RPS_Board.h RPS_FightInfo.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPS_ParserMove.o: RPS_ParserMove.cpp RPS_ParserMove.h 
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPS_ParserPosition.o: RPS_ParserPosition.cpp RPS_ParserPosition.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPS_Piece.o: RPS_Piece.cpp RPS_Piece.h 
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
#clean:
#	rm -f $*.o $(EXEC)

clean:
	rm -f $(OBJS) $(EXEC)
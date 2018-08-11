COMP = g++-5.3.0
OBJS = main.o RPSGame.o RPSParserMove.o RPSParserPosition.o Piece.o
EXEC = ex1
CPP_COMP_FLAG = -std=c++14 -Wall -Wextra -Werror -pedantic-errors -DNDEBUG

$(EXEC): $(OBJS)
	$(COMP) $(OBJS) $(CPP_COMP_FLAG) -o $@

main.o: main.cpp RPSGame.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPSGame.o: RPSGame.cpp RPSGame.h Piece.h LosingDetails.h RPSParserMove.h RPSParserPosition.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPSParserMove.o: RPSParserMove.cpp RPSParserMove.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPSParserPosition.o: RPSParserPosition.cpp RPSParserPosition.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Piece.o: Piece.cpp Piece.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
clean:
	rm -f $*.o $(EXEC)
COMP = g++-5.3.0
OBJS = main.o AlgorithmRegistration.o RPS_TournamentManager.o RPS_Board.o RPS_FightInfo.o RPS_Manager_ex3.o RPS_Piece.o 
EXEC = ex3
CPP_COMP_FLAG = -std=c++14 -Wall -Wextra -Werror -pedantic-errors -DNDEBUG


$(EXEC): $(OBJS)
	$(COMP) $(OBJS) -rdynamic -ldl -pthread -o $@
RSPPlayer_313534828.so: RSPPlayer_313534828.o RPS_Board.o RPS_Piece.o 
	$(COMP) RSPPlayer_313534828.o RPS_Board.o RPS_Piece.o  -shared -o $@
RSPPlayer_313534828.o: RSPPlayer_313534828.cpp RSPPlayer_313534828.h PlayerAlgorithm.h RPS_Move.h RPS_JokerChange.h RPS_PiecePosition.h RPS_Board.h AlgorithmRegistration.h
	$(COMP) -fPIC $(CPP_COMP_FLAG) -c $*.cpp
main.o: main.cpp RPS_TournamentManager.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPS_Board.o: RPS_Board.cpp RPS_Board.h Board.h RPS_Piece.h Point.h
	$(COMP) -fPIC $(CPP_COMP_FLAG) -c $*.cpp
RPS_FightInfo.o: RPS_FightInfo.cpp RPS_FightInfo.h RPS_Point.h FightInfo.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPS_Manager_ex3.o: RPS_Manager_ex3.cpp RPS_Manager_ex3.h PlayerAlgorithm.h RPS_Board.h RPS_FightInfo.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPS_Piece.o: RPS_Piece.cpp RPS_Piece.h 
	$(COMP) -fPIC $(CPP_COMP_FLAG) -c $*.cpp
RPS_TournamentManager.o: RPS_TournamentManager.cpp RPS_TournamentManager.h RPS_Manager_ex3.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
AlgorithmRegistration.o: AlgorithmRegistration.cpp AlgorithmRegistration.h RPS_TournamentManager.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
.PHONY: all
all: $(EXEC) RSPPlayer_313534828.so
.PHONY: rps_tournament
rps_tournament: $(EXEC)
.PHONY: rps_lib
rps_lib: RSPPlayer_313534828.so
clean:
	rm -f $(OBJS) $(EXEC) RSPPlayer_313534828.o RSPPlayer_205364730.o RSPPlayer_205364730.so RSPPlayer_313534828.so
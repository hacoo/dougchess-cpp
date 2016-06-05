// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 18 Apr. 2016
// 
// dougchess/Alphabeta.cpp
// 
// Alpha-Beta minichess player. 
 

#ifndef ALPHABETA_H
#define ALPHABETA_H


#include <string>
#include <stdio.h>
#include <cstring>
#include <time.h>
#include <exception>
#include "rules.h"
#include "Movegen.h"
#include "SimpleEval.h"
#include "Board.h"
#include "TimeManager.h"
#include "ZobristTable.h"
#include "TranspositionTable.h"


extern std::atomic<bool> currently_pondering_atom;
extern std::atomic<bool> continue_pondering_atom;

Move alphabeta_move(const Board& board,
		    int depth,
		    TimeManager& manager,
		    ZobristTable& zobrist,
		    TranspositionTable& tt,
		    bool ponderMode);

int alphabeta_move_score(Board& board,
			 int depth,
			 int alpha,
			 int beta,
			 TimeManager& manager,
			 ZobristTable& zobrist,
			 u64 tt_hash,
			 TranspositionTable& tt,
			 bool ponderMode);

int tt_lookup(TranspositionTable& tt,
	      u64 hash,
	      int depth,
	      int& alpha,
	      int& beta,
	      int& score);
void tt_store(TranspositionTable& tt,
	      u64 hash,
	      int depth,
	      int old_alpha,
	      int beta,
	      int score);


 
#endif

// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 18 Apr. 2016
// 
// dougchess/Negamax.cpp
// 
// Negamax minichess player. 
 

#ifndef NEGAMAX_H
#define NEGAMAX_H


#include <string>
#include <stdio.h>
#include <cstring>
#include <time.h>
#include "rules.h"
#include "Movegen.h"
#include "SimpleEval.h"
#include "Board.h"


Move negamax_move(const Board& board,
		  int depth);
int negamax_move_score(Board& board,
		       int depth);

 
#endif

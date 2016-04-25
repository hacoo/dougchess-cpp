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


Move alphabeta_move(const Board& board,
		    int depth,
		    TimeManager& manager);
int alphabeta_move_score(Board& board,
			 int depth,
			 int alpha,
			 int beta,
			 TimeManager& manager);

 
#endif

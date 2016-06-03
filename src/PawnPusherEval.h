// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 18 Apr. 2016
// 
// dougchess/PawnPusherEval.cpp

// Simple evaluator. Takes into account position. Values pawn
// advancement.


#ifndef PAWNPUSHEREVAL_H
#define PAWNPUSHEREVAL_H

#include <string>
#include <stdio.h>
#include <cstring>
#include <time.h>
#include "rules.h"

#define PAWN_PUSH_VALUE 10;


class PawnPusherEval {
 public :
  PawnPusherEval() {}
  ~PawnPusherEval() {}

  static int eval(const char board[RANKS][FILES], char player) {
    float acc = 0.0;
    char piece;
    
    // This is a little ugly -- but using a switch statement is
    // very fast
    if (player == 'W') {
      for (int y = 0; y < RANKS; ++y) {
	for (int x = 0; x < FILES; ++x) {
	  
	  piece = board[y][x];
	  
	  switch (piece) {
	    
	  case '.': break;
	    
	  case 'p':
	    acc -= 100;
	    acc -= y * PAWN_PUSH_VALUE;
	    break;
	  case 'n': acc -= 300; break;
	  case 'b': acc -= 300; break;
	  case 'r': acc -= 500; break;
	  case 'q': acc -= 900; break;
	  case 'k': acc -= 100000; break;	    
	  case 'P':
	    acc += 100;
	    acc += (RANKS - 1 - y) * PAWN_PUSH_VALUE;
	    break;
	  case 'N': acc += 300; break;
	  case 'B': acc += 300; break;
	  case 'R': acc += 500; break;
	  case 'Q': acc += 900; break;
	  case 'K': acc += 100000; break;
	    
	  }
	}
      }
    } else {
      for (int y = 0; y < RANKS; ++y) {
	for (int x = 0; x < FILES; ++x) {
	  
	  piece = board[y][x];
	  
	  switch (piece) {
	    
	  case '.': break;
	    
	  case 'p':
	    acc += 100;
	    acc += y * PAWN_PUSH_VALUE;
	    break;
	  case 'n': acc += 300; break;
	  case 'b': acc += 300; break;
	  case 'r': acc += 500; break;
	  case 'q': acc += 900; break;
	  case 'k': acc += 100000; break;
	    
	  case 'P':
	    acc -= 100;
	    acc -= (RANKS - 1 - y) * PAWN_PUSH_VALUE;
	    break;
	  case 'N': acc -= 300; break;
	  case 'B': acc -= 300; break;
	  case 'R': acc -= 500; break;
	  case 'Q': acc -= 900; break;
	  case 'K': acc -= 100000; break;
	    
	  }
	}
      }
    }
    return acc;
  }  
};








#endif

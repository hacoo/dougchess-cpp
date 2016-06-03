// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 18 Apr. 2016
// 
// dougchess/SimpleEval.h
// 
// Very simple evaluator. Takes into account material ONLY.


#ifndef SIMPLEEVAL_H
#define SIMPLEEVAL_H

#include <string>
#include <stdio.h>
#include <cstring>
#include <time.h>
#include "rules.h"

// Scoring array. Acts as a global lookup table assosciating a piece
// to a score.

// TODO: Make an awesome super fast version of this in assembly??
// This needs to optimized to the max!

class SimpleEval {
 public :
  SimpleEval() {}
  ~SimpleEval() {}

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
	    
	  case 'p': acc -= 100; break;
	  case 'n': acc -= 300; break;
	  case 'b': acc -= 300; break;
	  case 'r': acc -= 500; break;
	  case 'q': acc -= 900; break;
	  case 'k': acc -= 100000; break;
	    
	  case 'P': acc += 100; break;
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
	    
	  case 'p': acc += 100; break;
	  case 'n': acc += 300; break;
	  case 'b': acc += 300; break;
	  case 'r': acc += 500; break;
	  case 'q': acc += 900; break;
	  case 'k': acc += 100000; break;
	    
	  case 'P': acc -= 100; break;
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

// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 6 Apr. 2016
// 
// dougchess/Eval.h
// 
// Minichess evaluator class. Holds the current evaluation for a 
// board.

#ifndef EVAL_H
#define EVAL_H

#include <string>
#include <cstring>
#include <iostream>
#include "rules.h"

class Eval {
 public:
  Eval();
  ~Eval();
  static int score_simple(const char board[RANKS][FILES], char player);
  
 private:
};


#endif

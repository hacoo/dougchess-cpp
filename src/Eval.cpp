// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 6 Apr. 2016
//
// dougchess/Eval.cpp
// 
// Minichess evaluator class. Holds the current evaluation for a 
// board.

#include "Eval.h"

Eval::Eval() {
}

Eval::~Eval(){
}

int Eval::score_simple(const char board[RANKS][FILES], char player) {
  int p = (player == 'W') ? 0 : 1;  
  float acc = 0.0;
  for (int y = 0; y < RANKS; ++y) {
    for (int x = 0; x < FILES; ++x) {
      acc += simple_score_table.scores[p][(int)board[y][x]];
    }
  }
  return acc;
}



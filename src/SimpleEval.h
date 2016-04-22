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


class SimpleScoreTable {
 public:
  SimpleScoreTable() {
    scores[0]['k'] = -100000;
    scores[0]['K'] = 100000;
    scores[0]['q'] = -900;
    scores[0]['Q'] = 900;
    scores[0]['r'] = -500;
    scores[0]['R'] = 500;
    scores[0]['b'] = -300;
    scores[0]['B'] = 300;
    scores[0]['n'] = -300;
    scores[0]['N'] = 300;
    scores[0]['p'] = -100;
    scores[0]['P'] = 100;
    scores[0]['.'] = 0;

    scores[1]['k'] = 100000;
    scores[1]['K'] = -100000;
    scores[1]['q'] = 900;
    scores[1]['Q'] = -900;
    scores[1]['r'] = 500;
    scores[1]['R'] = -500;
    scores[1]['b'] = 300;
    scores[1]['B'] = -300;
    scores[1]['n'] = 300;
    scores[1]['N'] = -300;
    scores[1]['p'] = 100;
    scores[1]['P'] = -100;
    scores[1]['.'] = 0;
  }
  int scores[2][128];
};

// Scoring array. Acts as a global lookup table assosciating a piece
// to a score.
static SimpleScoreTable simple_score_table;


class SimpleEval {
 public :
  SimpleEval() {}
  ~SimpleEval() {}

  static int eval(const char board[RANKS][FILES], char player) {
    int p = (player == 'W') ? 0 : 1;  
    float acc = 0.0;
    for (int y = 0; y < RANKS; ++y) {
      for (int x = 0; x < FILES; ++x) {
	acc += simple_score_table.scores[p][(int)board[y][x]];
      }
    }
    return acc;
  }
  
};


#endif


// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 3 Apr. 2016
// 
// dougchess/rules.h
// 
// Invariant minichess rules and constants.

#ifndef RULES_H
#define RULES_H

#define RANKS 6
#define FILES 5
#define DUMPSIZE 41
#define isValid(x, y) ((x > -1) && (x < FILES) && (y > -1) && (y < RANKS))



class SimpleScoreTable {
 public:
  SimpleScoreTable() {
    scores[0]['k'] = -100000.0;
    scores[0]['K'] = 100000.0;
    scores[0]['q'] = -9.0;
    scores[0]['Q'] = 9.0;
    scores[0]['r'] = -5.0;
    scores[0]['R'] = 5.0;
    scores[0]['n'] = -3.0;
    scores[0]['N'] = 3.0;
    scores[0]['p'] = -1.0;
    scores[0]['P'] = 1.0;
    scores[0]['.'] = 0.0;


    scores[1]['k'] = 100000.0;
    scores[1]['K'] = -100000.0;
    scores[1]['q'] = 9.0;
    scores[1]['Q'] = -9.0;
    scores[1]['r'] = 5.0;
    scores[1]['R'] = -5.0;
    scores[1]['n'] = 3.0;
    scores[1]['N'] = -3.0;
    scores[1]['p'] = 1.0;
    scores[1]['P'] = -1.0;
    scores[1]['.'] = 0.0;
  }
  float scores[2][128];
};

// Scoring array. Acts as a global lookup table assosciating a piece
// to a score.
static SimpleScoreTable simple_score_table;

#endif






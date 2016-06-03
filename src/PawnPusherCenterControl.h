// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 18 Apr. 2016
// 
// dougchess/PawnPusherCenterEval.h

// Slightly more complex evaluator, prefers pawn pushing and controlling
// the center for all other pieces.

#ifndef PAWNPUSHERCENTERCONTROL_H
#define PAWNPUSHERCENTERCONTROL_H

#include <string>
#include <stdio.h>
#include <cstring>
#include <time.h>
#include "rules.h"

#define PAWN_PUSH_VALUE 10

class PawnPusherCenterControlTable {
 public:
  PawnPusherCenterControlTable() {
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

// Mask indicating the 'added value' of each square
class BoardMask {
 public:
  BoardMask() {
    // Initialize the boardmask for each piece.
    // 0 is white, 1 is black.

    memset(mask, 0, sizeof(int)*2*128*RANKS*FILES);

    // Nonpawn pieces use the center control mask
    
    // Initialize for white:
    for (int c = 0; c < 128; ++c) {
      for (int y = 0; y < RANKS; ++y) {
	for (int x = 0; x < FILES; ++x) {
	  if (isupper(c)) {
 	    mask[0][c][y][x] = 10 - 5 * abs(2-x);
	    if (y > 2) 
	      mask[0][c][y][x] += 15 - 5 * abs(2-y);
	    else
	      mask[0][c][y][x] += 15 - 5 * abs(3-y);
	  } else {
	    mask[0][c][y][x] = -(10 - 5 * abs(2-x));
	    if (y > 2) 
	      mask[0][c][y][x] -= 15 - 5 * abs(2-y);
	    else
	      mask[0][c][y][x] -= 15 - 5 * abs(3-y);
	  }
	}
      }
    }
    
    // Same for black:
    for (int c = 0; c < 128; ++c) {
      for (int y = 0; y < RANKS; ++y) {
	for (int x = 0; x < FILES; ++x) {
	  if (isupper(c)) {
 	    mask[1][c][y][x] = -(10 - 5 * abs(2-x));
	    if (y > 2) 
	      mask[1][c][y][x] -= 15 - 5 * abs(2-y);
	    else
	      mask[1][c][y][x] -= 15 - 5 * abs(3-y);
	  } else {
	    mask[1][c][y][x] = 10 - 5 * abs(2-x);
	    if (y > 2) 
	      mask[1][c][y][x] += 15 - 5 * abs(2-y);
	    else
	      mask[1][c][y][x] += 15 - 5 * abs(3-y);
	  }
	}
      }
    }

    // Reset all the empty squares to 0
    for (int p = 0; p < 2; ++p) {
      for (int y = 0; y < RANKS; ++y) {
	for (int x = 0; x < FILES; ++x) {
	  mask[p]['.'][y][x] = 0;
	}
      }
    }

    // Overwrite pawns -- they should use the pawn advancement scheme
    for (int y = 0; y < RANKS; ++y) {
      for (int x = 0; x < FILES; ++x) {
        mask[0]['p'][y][x] = -PAWN_PUSH_VALUE * y;
	mask[1]['p'][y][x] = PAWN_PUSH_VALUE * y;
	mask[0]['P'][y][x] = PAWN_PUSH_VALUE * (RANKS-1-y);
	mask[1]['P'][y][x] = -PAWN_PUSH_VALUE * (RANKS-1-y);
      }
    }

    // Whew!
  }
  
  int mask[2][128][RANKS][FILES];
};


// Mask indicating the 'pawn push value'
class PawnMask {
 public:
 PawnMask()
   : mask 
	   {{{50, 50, 50, 50, 50},
	     {40, 40, 40, 40, 40},
	     {30, 30, 30, 30, 30},
	     {20, 20, 20, 20, 20},
	     {10, 10, 10, 10, 10},
	     {0,  0,  0,  0,  0}},

	    {{0,  0,  0,  0,  0},
	     {10, 10, 10, 10, 10},
	     {20, 20, 20, 20, 20},
	     {30, 30, 30, 30, 30},
	     {40, 40, 40, 40, 40},
	     {50, 50, 50, 50, 50}}}
  {}
  
  int mask[2][RANKS][FILES];
};

// Scoring array. Acts as a global lookup table assosciating a piece
// to a score.
static PawnPusherCenterControlTable score_table;
static BoardMask board_mask;
static PawnMask pawn_mask;


class PawnPusherCenterControl {
 public :
  PawnPusherCenterControl() {}
  ~PawnPusherCenterControl() {}

  static int eval(const char board[RANKS][FILES], char player) {
    int p = (player == 'W') ? 0 : 1;  
    float acc = 0.0;
    int max_rank_index = RANKS-1;
    
    for (int y = 0; y < RANKS; ++y) {
      for (int x = 0; x < FILES; ++x) {
	acc += score_table.scores[p][(int)board[y][x]];	
	char piece = board[y][x];
	acc += board_mask.mask[p][piece][y][x];	
      }
    }	
    return acc;
  }
};


#endif

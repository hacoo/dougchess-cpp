// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 18 Apr. 2016
// 
// dougchess/Negamax.cpp
// 
// Negamax minichess player. 

#include "negamax.h"

using namespace std;


// A deep copy of board is made for intermediate moves, so board 
// is NOT modified. 
Move negamax_move(const Board& board, int depth) {  
  
  // Deep copy the board to keep original safe:
  Board newboard(board);
  Move best_move;
  int best_score = -CHESSMAX;
  int score;
  
  // Kick off first search depth, since we need to catch the actual
  // move here

  vector<Move> ms = newboard.movesShuffled();

  for(auto i : ms) {
    newboard.move(i);
    score = -negamax_move_score(newboard, depth-1);
    newboard.undo();
    if (score > best_score) {
      best_score = score;
      best_move.clone(i);
    }
  }
  
  return best_move;
}

// Return the best possible score starting from board,
// and exploring depth moves
int negamax_move_score(Board& board, int depth) {  
  if(depth <= 0 || board.winner() != '?')
    return board.eval();

  vector<Move> ms = board.movesShuffled();
  int score = -CHESSMAX;
  for(auto i : ms) {
    board.move(i);
    score = max(-negamax_move_score(board, depth-1), score);
    board.undo();
  }
  return score;
}



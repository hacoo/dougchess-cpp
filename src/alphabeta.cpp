// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 18 Apr. 2016
// 
// dougchess/Negamax.cpp
// 
// Alpha-Beta minichess player. 


#include "alphabeta.h"

using namespace std;

Move alphabeta_move(Board& board, int depth) {

  Move best_move;
  int score = -CHESSMAX;  

  // Kick off first search depth, since we need to catch the actual
  // move here
  int alpha = -CHESSMAX;
  int beta = CHESSMAX;

  vector<Move> ms = board.movesEvaluated();

  for (auto i : ms) {
    board.move(i);
    score = -alphabeta_move_score(board, depth-1, -beta, -alpha);
    board.undo();
    if (score > alpha) {
      alpha = score;
      best_move.clone(i);
    }
  }
  
  return best_move;
}

// Get the best possible score using alphabeta search
int alphabeta_move_score(Board& board, int depth, int alpha, int beta) {
  if(depth <= 0 || board.winner() != '?') 
    return board.eval();
  
  vector<Move> ms = board.movesEvaluated();
  int score = -CHESSMAX;
  
  for(auto i : ms) {
    board.move(i);
    score = max(score, 
		-alphabeta_move_score(board, depth-1, -beta, -alpha));
    board.undo();
    alpha = max(alpha, score);  
    if (alpha >= beta) 
      return score;
  }
  
  return score;  
}

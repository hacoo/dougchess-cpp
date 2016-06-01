// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 18 Apr. 2016
// 
// dougchess/Negamax.cpp
// 
// Alpha-Beta minichess player. 


#include "alphabeta.h"

using namespace std;

Move alphabeta_move(const Board& board,
		    int depth,
		    TimeManager& manager,
		    ZobristTable& zobrist,
		    TranspositionTable& tt) {
		   
  Board newboard(board);
  Move best_move;
  int score = -CHESSMAX;  

  // Kick off first search depth, since we need to catch the actual
  // move here
  int alpha = -CHESSMAX;
  int beta = CHESSMAX;

  // Create the initial Zobrist hash
  u64 tt_hash = newboard.zobristHash();
  u64 next_hash;

  // Use the TT to order moves -- on the first search ONLY,
  // as this is very expensive
  vector<Move> ms = newboard.movesTT();
  

  for (auto i : ms) {
    
    if(manager.out_of_time()) {
      throw OutOfTimeException("Search time exceeded");
    }

    next_hash = newboard.updateHash(tt_hash, i);
    newboard.move(i);
    score = -alphabeta_move_score(newboard,
				  depth-1,
				  -beta,
				  -alpha,
				  manager,
				  zobrist,
				  next_hash,
				  tt);
    newboard.undo();
    if (score > alpha) {
      alpha = score;
      best_move.clone(i);
    }
  }
  
  return best_move;
}

// Get the best possible score using alphabeta search
int alphabeta_move_score(Board& board,
			 int depth,
			 int alpha,
			 int beta,
			 TimeManager& manager,
			 ZobristTable& zobrist,
			 u64 tt_hash,
			 TranspositionTable& tt) {
  
  if(depth <= 0 || board.winner() != '?') 
    return board.eval();
  
  int score = -CHESSMAX;
  u64 next_hash;
  int old_alpha = alpha;
  TranspositionEntry tt_entry;
  
  // Handle TT lookup 
  if (tt_lookup(tt, tt_hash, depth, alpha, beta, score))
    return score;

  // If the TT missed, generate a list of moves.
  // Don't bother using the TT to order moves, it's too slow
  vector<Move> ms = board.movesEvaluated();
 
  for(auto i : ms) {
    
    if(manager.out_of_time()) {
      throw OutOfTimeException("Search time exceeded");
    }
    
    next_hash = board.updateHash(tt_hash, i);
    board.move(i);
    score = max(score, 
		-alphabeta_move_score(board,
				      depth-1,
				      -beta,
				      -alpha,
				      manager,
				      zobrist,
				      next_hash,
				      tt));
    board.undo();

    
    alpha = max(alpha, score);  
    if (alpha >= beta) 
      break;
  }

  // Handle TT store
  tt_store(tt, tt_hash, depth, old_alpha, beta, score);
  
  return score;  
}


// Will attmpet to look up the current position hash in the
// transposition table. If the lookup succeeds, alpha,
// beta, and score will be upated in place.

// The function will return 1 if an exact score is determined.
int tt_lookup(TranspositionTable& tt,
	      u64 hash,
	      int depth,
	      int& alpha,
	      int& beta,
	      int& score) {

  TranspositionEntry tt_entry;
    
  try {
      tt_entry = tt.lookup(hash);

      if (tt_entry.depth >= depth) {
	
	if (tt_entry.node_type == 'E') {
	  score = tt_entry.score;
	  return 1;
	}
	  
 	else if (tt_entry.node_type == 'L') 
	  alpha = max(alpha, tt_entry.score);
	
	else if (tt_entry.node_type == 'U') 
	  beta = min(alpha, tt_entry.score);
	
	else {
	  cout << "ERROR -- unknown TT node type" << endl;
	  exit(1);
	}
	
	if (alpha >= beta) {
	  score = tt_entry.score;
	  return 1;
	}
      }
    } catch (TableMissException e) {
    ;
    }
  
  return 0; // Got partial information or no hit
}

// Store board information in the transposition table.
// The table is responsible for determining replacement policy.
void tt_store(TranspositionTable& tt,
	      u64 hash,
	      int depth,
	      int old_alpha,
	      int beta,
	      int score) {

  char node_type;
  if (score <= old_alpha)
    node_type = 'U';
  else if (score >= beta)
    node_type = 'L';
  else
    node_type = 'E';
  
  tt.store(hash, score, depth, node_type);
}


// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 13 Apr. 2016
// 
// dougchess/movegen.cpp
// 
// Move generation for dougchess.

#include "Movegen.h"
using namespace std;

Movegen::Movegen() {
}

Movegen::~Movegen() {
}


// Generate all moves and add them into vector ms.
void Movegen::generateMoves(const char board[RANKS][FILES],
			    const char player,
			    vector<Move>& ms) {
  for (int y = 0; y < RANKS; ++y) {
    for (int x = 0; x < FILES; ++x) {
      if(ownp(board[y][x], player))
	insertPieceMove(board, player, y, x, ms);
    }
  }
}


// Insert move for piece at x, y
void Movegen::insertPieceMove(const char boart[RANKS][FILES],
			      const char player,
			      int rank,
			      int file,
			      vector<Move>& ms) {
  
}


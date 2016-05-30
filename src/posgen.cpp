// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 28 May 2016
// 
// dougchess/posgen.cpp
// 
// Functions for generating positions. Operates on simple char
// arrays (not full board objects)

#include "posgen.h"

using namespace std;


vector<lwBoard*>
generateAllPositions(vector<char>& pieces) {
  
  vector<lwBoard*> positions;
  
  //Create blank starting board
  lwBoard* b = new lwBoard;
  for (int y = 0; y < RANKS; ++y) {
    for(int x = 0; x < FILES; ++x) {
      b->board[y][x] = '.';
    }
  }


  generateAllPositions(b, pieces, positions);
  
  return positions;
}

// Generate all remaining combinations of pieces, add each combination
// as its own board in the vector positions
void generateAllPositions(lwBoard* board,
			  vector<char>& pieces,
			  vector<lwBoard*>& positions) {
  // Base case -- if no more pieces to add, add in the modified board
  if(pieces.empty()) {
    positions.push_back(board);
    return;
  }

  char p = pieces.back();
  pieces.pop_back();
 
  // Otherwise, t\ake the first piece, and create a board with it
  // added at every availabe position
  for (int y = 0; y < RANKS; ++y) {
    for(int x = 0; x < FILES; ++x) {
      if (board->board[y][x]== '.') {
	lwBoard* newboard = new lwBoard;
	memcpy(newboard, board, sizeof(char)*RANKS*FILES);
	//newboard[y*FILES + x] = p;
	newboard->board[y][x] = p;
	vector<char> newvec = pieces;
	generateAllPositions(newboard, newvec, positions);
      }
    }
  }
  delete board;
}




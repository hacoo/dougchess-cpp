// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 28 May 2016
// 
// dougchess/posgen.cpp
// 
// Functions for generating positions. Operates on simple char
// arrays (not full board objects)

#include "posgen.h"

using namespace std;


vector<char*>
generateAllPositions(vector<char>& pieces) {
  
  vector<char*> positions;
  
  // Create blank starting board
  char* b = new char[RANKS*FILES];
  for (int y = 0; y < RANKS; ++y) {
    for(int x = 0; x < FILES; ++x) {
      b[y*FILES + x] = '.';
    }
  }

  generateAllPositions(b, pieces, positions);
  
  return positions;
}

// Generate all remaining combinations of pieces, add each combination
// as its own board in the vector positions
void generateAllPositions(char* board,
			  vector<char>& pieces,
			  vector<char*>& positions) {
  // Base case -- if no more pieces to add, add in the modified board
  if(pieces.empty()) {
    positions.push_back(board);
    return;
  }

  char p = pieces.back();
  pieces.pop_back();
 
  // Otherwise, take the first piece, and create a board with it
  // added at every availabe position
  for (int y = 0; y < RANKS; ++y) {
    for(int x = 0; x < FILES; ++x) {
      if (board[y*FILES + x] == '.') {
	char* newboard = new char[RANKS*FILES];
	memcpy(newboard, board, sizeof(char)*RANKS*FILES);
	newboard[y*FILES + x] = p;
	vector<char> newvec = pieces;
	generateAllPositions(newboard, newvec, positions);
      }
    }
  }
  delete[] board;
}




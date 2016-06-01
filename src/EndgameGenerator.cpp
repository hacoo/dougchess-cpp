// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 28 May 2016
// 
// dougchess/EndgameGenerator.cpp
// 
// Generates endgame positions for a given piece set. Will store them
// in a MongoDB database for later use.
//
// The endgame generator ALWAYS assumes that the player is white.
// Thus, boards will need to be converted before querying the
// endgame tablebase, if you're playing as black.
// Hopefully this isn't too slow.

#include "EndgameGenerator.h"

using namespace std;

EndgameGenerator::EndgameGenerator(const vector<char> piecelist) {
  pieces = piecelist;
  stringstream sstream;
  for (auto c : piecelist) {
    sstream << c;
  }
  tag = sstream.str();
}

EndgameGenerator::~EndgameGenerator() {
  for (auto b : positions) {
    delete[] b;
  }
}


// Create all endgame positions for this set of pieces
void EndgameGenerator::generate() {
  positions = generateAllPositions(pieces);
  // Allocate the list of endgame entries, now that
  // the number is known
  entries = new EndgameEntry[positions.size()];
  numEntries = positions.size();

  // Initialize the database -- create an 'infinite' entry for each move
  initDatabase();

  // Start by seeding checkmate moves
  seedCheckmates();
   
}

string EndgameGenerator::toString() {
  stringstream sstream;
  cout << positions.size() << endl;
  for (auto b : positions) {
    sstream << lwBoard_to_string(*b) << "\n";
  }
  return sstream.str();
}

// Returns the number of generated positions
unsigned long EndgameGenerator::numPositions() {
  return positions.size();
}

// Seed all positions where white checks black as '1' move from
// checkmate (since under minichess rules we must actually capture
// the king). Store the appropriate move.
void EndgameGenerator::seedCheckmates() {
  int i = 0;
  for (auto p : positions) {    
    if (blackKingThreatenedOnBoard(*p, entries[i].move)) {
	entries[i].to_mate = 1;
    }
    ++i;
  }

  return;
}

// Returns true if white threatens black's queen
// on this board. Returns the result in result_move.
bool EndgameGenerator::
blackKingThreatenedOnBoard(const lwBoard& board,
			   Move& result_move) {
  vector<Move> ms;
  Movegen::generateMoves(board.board, 'W', ms);
  for (auto m : ms) {
    if(moveThreatensBlackKing(board, m)) {
      result_move = m;
      return true;
    }
  }
  return false;
}

bool EndgameGenerator::
moveThreatensBlackKing(const lwBoard& board,
		       const Move& m) {
  int x = m.finish.file;
  int y = m.finish.rank;
  if (board.board[y][x] == 'k')
    return true;
  return false;
}

// Create a database entry for each position,
// initialize it to infinite depth-to-mate
void EndgameDatabase::initDatabase() {
  
}

// Copy the current state of this endgame table into the
// permanent database. Will iterate over all positions, this
// could be slow.
void EndgameGenerator::updateDatabase() {
  for (int i = 0; i < numEntries; ++i) {
    ;
  }
}

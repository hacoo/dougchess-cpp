// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 28 May 2016
// 
// dougchess/EndgameGenerator.cpp
// 
// Generates endgame positions for a given piece set. Will store them
// in a MongoDB database for later use.

#include "EndgameGenerator.h"

using namespace std;

EndgameGenerator::EndgameGenerator(vector<char> piecelist) {
  pieces = piecelist;
}

EndgameGenerator::~EndgameGenerator() {
  for (auto b : positions) {
    delete[] b;
  }
}


// Create all endgame positions for this set of pieces
void EndgameGenerator::generate() {
  positions = generateAllPositions(pieces);
}

string EndgameGenerator::toString() {
  stringstream sstream;
  cout << positions.size() << endl;
  for (auto b : positions) {
    sstream << flat_board_to_string(b) << "\n";
  }
  return sstream.str();
}

unsigned long EndgameGenerator::numPositions() {
  return positions.size();
}

// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 28 May 2016
// 
// dougchess/testEndgame.cpp
// 
// Functions for testing / playing with the endgame gen


#include "testEndgame.h"

using namespace std;

void testEndgame() {
  vector<char> pieces;
  pieces.push_back('k');
  pieces.push_back('K');
  //pieces.push_back('R');
  //pieces.push_back('r');
  
  EndgameGenerator eg(pieces);
  eg.generate();
  cout << eg.numPositions() << "\n";
  vector<lwBoard*> positions = eg.positions;
  stringstream sstream;
  Move result_move;
  int c = 0;
  for(auto i : positions) {
    
    if(EndgameGenerator::blackKingThreatenedOnBoard(*i, result_move)) {
      cout << lwBoard_to_string(*i) << "\n";
      cout << eg.entries[c].move.toString() << "\n";
      cout << eg.entries[c].to_mate << "\n";
    }
    ++c;
  }

  // for (int i = 0; i < positions.size(); ++i) {
  //   if (eg.entries[i].to_mate == 1) {
  //     cout << lwBoard_to_string(*positions[i]) << "\n";
  //     cout << eg.entries[i].move.toString() << "\n";
      
  //   }
    
  // }

  cout << sstream.str();
  
  //string bstrings = eg.toString();
  //cout << bstrings;
}


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
  //string bstrings = eg.toString();
  //cout << bstrings;
}


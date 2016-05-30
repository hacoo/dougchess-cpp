// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 28 May 2016
// 
// dougchess/EndgameGenerator.cpp
// 
// Generates endgame positions for a given piece set. Will store them
// in a MongoDB database for later use.

#ifndef ENDGAMEGENERATOR_H
#define ENDGAMEGENERATOR_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "rules.h"
#include "Board.h"
#include "posgen.h"
#include "utility.h"
#include "Movegen.h"


class EndgameGenerator {  
 public:
  EndgameGenerator(const std::vector<char> piecelist);
  ~EndgameGenerator();
  std::string toString();
  unsigned long numPositions();
  void generate();
  void seedCheckmates();
  static bool blackKingThreatenedOnBoard(const lwBoard& board,
					 Move& result_move);
  static bool moveThreatensBlackKing(const lwBoard& board,
				     const Move& m);
  std::vector<lwBoard*> positions;
  EndgameEntry* entries;

 private:
  std::vector<char> pieces;
}; 




#endif



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

class EndgameGenerator {
  
 public:
  EndgameGenerator(std::vector<char> piecelist);
  ~EndgameGenerator();
  std::string toString();
  unsigned long numPositions();
  void generate();

 private:
  std::vector<char> pieces;
  std::vector<char*> positions;
}; 




#endif



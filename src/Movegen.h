// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 13 Apr. 2016
// 
// dougchess/movegen.cpp
// 
// Move generation for dougchess.



#ifndef MOVEGEN_H
#define MOVEGEN_H

#include <string>
#include <iostream>
#include <cstring>
#include <vector>
#include "rules.h"
#include "utility.h"

class Movegen {
 public: 
  Movegen();
  ~Movegen();
  static void generateMoves(const char board[RANKS][FILES],
			    const char player,
			    std::vector<Move>& ms);

  static void insertPieceMove(const char board[RANKS][FILES],
			      const char player,
			      int rank,
			      int file,
			      std::vector<Move>& ms);

  
 private:
  
};


#endif

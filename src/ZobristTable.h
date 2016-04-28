// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 25 Apr. 2016
// 
// dougchess/ZobristTable.h
// 
// Table of Zobrist numbers for each square/piece combination.

#ifndef ZOBRISTTABLE_H
#define ZOBRISTTABLE_H

#include <random>
#include <chrono>
#include <string>
#include <sstream>
#include "rules.h"



class ZobristTable {

 public:
  ZobristTable();
  ZobristTable(unsigned int seed);
  ZobristTable(const ZobristTable& other);
  ~ZobristTable();
  u64 get(int rank, int file, char piece) const;
  std::string toString() const;  
  ZobristTable& operator = (const ZobristTable& other);
  u64 hash_board(const char board[RANKS][FILES],
		 const char player) const;
  u64 update_hash(const u64 old,
		  const char board[RANKS][FILES],
		  const char player,
		  const Move& move) const;
  
    
 private:
  u64 table[RANKS][FILES][128];
  u64 player_code[128];

};


#endif

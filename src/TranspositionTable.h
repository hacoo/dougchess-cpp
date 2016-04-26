// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 25 Apr. 2016
// 
// dougchess/TranspositionTable.h
// 
// Minichess transposition table. Should be global (?) like the
// time manager, so all Boards can share it (will need to be threadsafe
// though...)

#ifndef TRANSPOSITIONTABLE_H
#define TRANSPOSITIONTABLE_H

#include <string>
#include <stdio.h>
#include <cstring>
#include <sstream>
#include <vector>
#include <chrono>
#include <atomic>
#include "rules.h"
#include "ZobristTable.h"

#define TT_SIZE 16777216 // 2 ^ 24
//#define TT_SIZE 10 // 2 ^ 24


// Data storage class for TT entries
class TranspositionEntry {
 public:
  TranspositionEntry() :
    valid(false),
    hash(0),
    score(0),
    depth(0),
    node_type('?') {}
    
  bool valid;
  u64 hash;
  int score;
  unsigned short int depth;
  char node_type; 
  
 private:
  
};


class TranspositionTable {
 public:
  TranspositionTable(ZobristTable& zobrist);
  ~TranspositionTable();
  int lookup(u64 hash);
  void store(u64 hash, int score);
  void clear();

 private:
  TranspositionEntry* table;
  ZobristTable& zobrist;
};

#endif



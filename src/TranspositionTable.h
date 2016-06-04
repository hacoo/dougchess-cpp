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

//#define TT_SIZE 536870912 // 2^29
#define TT_SIZE 268435456 // 2^28
//#define TT_SIZE 67108864 // 2 ^ 26
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
  TranspositionTable();
  ~TranspositionTable();
  int lookup(const u64 hash, TranspositionEntry*& entry);
  void store(const u64 hash,
	     const int score,
	     const int depth,
	     const char node_type);
  void clear();
  u64 getMisses() const;
  u64 getConflicts() const;
  u64 getHits() const;
  u64 getReplacements() const;
  u64 getStores() const;

 private:
  u64 misses;
  u64 conflicts;
  u64 hits;
  u64 replacements;
  u64 stores;
  TranspositionEntry* table;
  
};

#endif



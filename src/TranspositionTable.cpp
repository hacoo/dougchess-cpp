// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 26 Apr. 2016
// 
// dougchess/TranspositionTable.h
// 
// Minichess transposition table. Should be global (?) like the
// time manager, so all Boards can share it (will need to be threadsafe
// though...)

#include "TranspositionTable.h"

using namespace std;

TranspositionTable::TranspositionTable() :
  misses(0), conflicts(0), hits(0), replacements(0) {
  table = new TranspositionEntry[TT_SIZE];
}

TranspositionTable::~TranspositionTable() {
  delete table;
}

void TranspositionTable::clear() {
  for(u64 i = 0; i < TT_SIZE; ++i) {
    table[i].valid = false;
    table[i].hash  = 0;
    table[i].score = 0;
    table[i].depth = 0;
    table[i].node_type  = '?'; 
  }  
}

// looks up a TT entry, and sets entry to point to it if found.
// Returns 1 on success, 0 on failure.
int TranspositionTable::lookup(const u64 hash,
			       TranspositionEntry*& entry) {

  int i = hash % TT_SIZE;
  
  if(table[i].valid == false) {
    misses += 1;
    return 0;
  }

  if(table[i].hash != hash) {
    conflicts += 1;
    return 0;
  }

  hits += 1;
  entry = &table[i];
  return 1;
}

void TranspositionTable::store(const u64 hash,
			       const int score,
			       const int depth,
			       const char node_type) {

  int i = hash % TT_SIZE;
  
  if(table[i].valid && table[i].hash != hash) {
    if (table[i].depth > depth)
      return; // Prefer best depth
    else
      replacements += 1;
  }

  table[i].hash      = hash;
  table[i].valid     = true;
  table[i].score     = score;
  table[i].depth     = depth;
  table[i].node_type = node_type;
  stores += 1;
}


u64 TranspositionTable::getHits() const {
  return hits;
}

u64 TranspositionTable::getConflicts() const {
  return conflicts;
}

u64 TranspositionTable::getReplacements() const {
  return replacements;
}

u64 TranspositionTable::getMisses() const {
  return misses;
}

u64 TranspositionTable::getStores() const {
  return stores;
}

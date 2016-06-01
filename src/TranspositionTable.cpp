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

TranspositionEntry& TranspositionTable::lookup(u64 hash)
  throw(TableMissException) {

  int i = hash % TT_SIZE;
  
  if(table[i].valid == false) {
    misses += 1;
    throw TableMissException("Transposition Table Entry Invalid");
  }

  if(table[i].hash != hash) {
    conflicts += 1;
    throw TableMissException("Transposition Table Entry Conflict");
  }

  hits += 1;
  return table[i];
}

void TranspositionTable::store(u64 hash,
			       int score,
			       int depth,
			       char node_type) {

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


u64 TranspositionTable::getHits() {
  return hits;
}

u64 TranspositionTable::getConflicts() {
  return conflicts;
}

u64 TranspositionTable::getReplacements() {
  return replacements;
}

u64 TranspositionTable::getMisses() {
  return misses;
}

u64 TranspositionTable::getStores() {
  return stores;
}

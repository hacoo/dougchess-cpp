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

TranspositionEntry TranspositionTable::lookup(u64 hash)
  throw(TableMissException) {
  
  if(!table[hash].valid) {
    misses += 1;
    throw TableMissException("Transposition Table Entry Invalid");
  }

  if(!table[hash].hash != hash) {
    conflicts += 1;
    throw TableMissException("Transposition Table Entry Conflict");
  }

  hits += 1;
  return table[hash];
}

void TranspositionTable::store(u64 hash,
			       int score,
			       int depth,
			       char node_type) {
  if(table[hash].valid && table[hash].hash == hash)
    replacements += 1;

  table[hash].hash      = hash;
  table[hash].valid     = true;
  table[hash].score     = score;
  table[hash].depth     = depth;
  table[hash].node_type = node_type;
  
}

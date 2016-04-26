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

TranspositionTable::TranspositionTable(ZobristTable& zobrist)
  : zobrist(zobrist) {
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




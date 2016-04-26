// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 25 Apr. 2016
// 
// dougchess/ZobristTable.h
// 
// Table of Zobrist numbers for each square/piece combination.


#include "ZobristTable.h"

using namespace std;

// Default constructor -- use time to seed
ZobristTable::ZobristTable() {
  
  mt19937_64 engine(chrono::system_clock::
		    now().time_since_epoch().count());
  std::uniform_int_distribution<unsigned long long> distr;
  
    for (int r = 0; r < RANKS; ++r) {
      for (int c = 0; c < FILES; ++ c) {
	for (int p = 0; p < DISTINCT_PIECES; ++p) {
	  table[r][c][p] = distr(engine);
      }
    }
  }
}

ZobristTable::ZobristTable(const ZobristTable& other) {
  *this = other;
}

ZobristTable::ZobristTable(unsigned int seed) {
  
  mt19937_64 engine(seed);
		    
  std::uniform_int_distribution<unsigned long long> distr;
  
    for (int r = 0; r < RANKS; ++r) {
      for (int c = 0; c < FILES; ++ c) {
	for (int p = 0; p < DISTINCT_PIECES; ++p) {
	  table[r][c][p] = distr(engine);
      }
    }
  }
}

ZobristTable::~ZobristTable() { }

ZobristTable& ZobristTable::operator = (const ZobristTable& other) {
  
  for (int r = 0; r < RANKS; ++r) {
    for (int c = 0; c < FILES; ++ c) {
      for (int p = 0; p < DISTINCT_PIECES; ++p) {
	table[r][c][p] = other.table[r][c][p];
      }
    }
  }
  return *this;
}


u64 ZobristTable::get(int rank, int file, char piece) const {
  
  return table[rank][file][(int) piece];
}


string ZobristTable::toString() const {
  
  stringstream sstream;
  
  for (int r = 0; r < RANKS; ++r) {
    for (int c = 0; c < FILES; ++ c) {
      for (int p = 0; p < DISTINCT_PIECES; ++p) {
	sstream << table[r][c][p] << "\n";
      }
    }
  }

  return sstream.str();
}

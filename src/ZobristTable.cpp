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
      for (int p = 0; p < 128; ++p) {
	table[r][c][p] = distr(engine);
      }
    }
  }
  player_code['W'] = distr(engine);
  player_code['B'] = distr(engine);
}

ZobristTable::ZobristTable(const ZobristTable& other) {
  *this = other;
}

ZobristTable::ZobristTable(unsigned int seed) {
  
  mt19937_64 engine(seed);
		    
  std::uniform_int_distribution<unsigned long long> distr;
  
  for (int r = 0; r < RANKS; ++r) {
    for (int c = 0; c < FILES; ++ c) {
      for (int p = 0; p < 128; ++p) {
	table[r][c][p] = distr(engine);
      }
    }
  }
  player_code['W'] = distr(engine);
  player_code['B'] = distr(engine);
}

ZobristTable::~ZobristTable() { }

ZobristTable& ZobristTable::operator = (const ZobristTable& other) {
  
  for (int r = 0; r < RANKS; ++r) {
    for (int c = 0; c < FILES; ++ c) {
      for (int p = 0; p < 128; ++p) {
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
      for (int p = 0; p < 128; ++p) {
	sstream << table[r][c][p] << "\n";
      }
    }
  }

  return sstream.str();
}


// Create a new zobrist hash for a board
u64 ZobristTable::hash_board(const char board[RANKS][FILES],
			     const char player) const {
  u64 hash = player_code[player];
  char piece;
  for (int y = 0; y < RANKS; ++y) {
    for (int x = 0; x < FILES; ++x) {
      piece = board[y][x];
      hash ^= table[y][x][piece];
    }
  }
  
  
  return hash;
}

// Update the hash with move TO BE MADE and return.
// Must be executed before the move is actually
// made! BEFORE!
u64 ZobristTable::update_hash(const u64 old,
			      const char board[RANKS][FILES],
			      const char player,
			      const Move& move) const {

  int srank = move.start.rank;
  int sfile = move.start.file;
  int frank = move.finish.rank;
  int ffile = move.finish.file;
  char start_piece = board[srank][sfile];
  char finish_piece = board[frank][ffile];
  
  u64 updated = old;
  
  updated ^= table[srank][sfile][start_piece];
  updated ^= table[srank][sfile]['.'];

  updated ^= table[frank][ffile][finish_piece];
  updated ^= table[frank][ffile][start_piece];

  if (player == 'W') { 
    updated ^= player_code['W'];
    updated ^= player_code['B'];
  } else {
    updated ^= player_code['B'];
    updated ^= player_code['W'];
  }

  return updated;
}

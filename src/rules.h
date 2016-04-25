// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 3 Apr. 2016
// 
// dougchess/rules.h
// 
// Invariant minichess rules and constants.
// Also contains low level representations of things like moves,
// squares

#ifndef RULES_H
#define RULES_H
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <cstdlib>
//#include <gperftools/profiler.h>

#define CHESSMAX 100000000
#define MAX_TURNS 40

inline std::vector<std::string>& _split(const std::string& s,
					char delim,
					std::vector<std::string>& elems) {
  std::stringstream sstream(s);
  std::string item;
  while(getline(sstream, item, delim)) {
    if(!item.empty())
      elems.push_back(item); 
  }
  return elems;
}

inline std::vector<std::string> split(const std::string& s, char delim) {
  std::vector<std::string> elems;
  _split(s, delim, elems);
  return elems;
}


#define RANKS 6
#define FILES 5
#define DUMPSIZE 41
#define isValid(y, x) ((x > -1) && (x < FILES) && (y > -1) && (y < RANKS))
#define rankLookup(r) (RANKS-r) + 48
#define fileLookup(f) f + 97 
#define ownp(piece, owner) ((owner == 'W') ? isupper(piece) : islower(piece))
#define enemyp(piece, owner) ((owner == 'W') ? islower(piece) : isupper(piece))
#define emptyp(piece) (piece == '.')




// Represents a particular board square.
// This is a lightweight data-container with PUBLIC data.
class Square {
 public:
  Square() : rank(0), file(0) {}
  Square(int rank, int file) : rank(rank), file(file) {}

  Square(const Square& other) : rank(other.rank), file(other.file) {}

  Square(const std::string& s) {
    char f = s[0];
    char r = s[1];
    rank = -((int) r) + 48 + RANKS;
    file = (int) f - 97;
  }
  
  ~Square() {}

  std::string toString() const {
    char temp[4] = { (char) fileLookup(file),
		     (char) rankLookup(rank),
		     '\0' };
    return std::string(temp);
  }

  int getRank() { return rank; }
  int getFile() { return file; }

  int rank;
  int file;
};


// Represents a move from one square to another.
// This is a lightweight data-container with PUBLIC data.
class Move {
 public:
  Move() {}
  Move(Square start, Square finish) : start(start), finish(finish) {}

  Move (const Move& other) : start(other.start),
    finish(other.finish) {}

  // construct from stringvq
  Move(const std::string& m){
    std::vector<std::string> splits = split(m, '-');
    start = Square(splits[0]);
    finish = Square(splits[1]);
  }

  ~Move() {}
  
  void clone(const Move& other) {
    start.rank = other.start.rank;
    start.file = other.start.file;
    finish.rank = other.finish.rank;
    finish.file = other.finish.file;
  }

  // Turn this move into the reverse of other
  void reverse(const Move& other) {
    start.rank = other.finish.rank;
    start.file = other.finish.file;
    finish.rank = other.start.rank;
    finish.file = other.start.file;
  }

  std::string toString() const {
    return start.toString() + "-" + finish.toString() + "\n";
  }

  Square start;
  Square finish;
};


// Simple container class for holding both a 
// move and the score that results. 
class ScoredMove {
 public: 
  ScoredMove(): score(-CHESSMAX) {}
  ~ScoredMove() {}
  ScoredMove(const Move& m, int s) : move(m), score(s) {}
  
  Move move;
  int score;
};


#endif






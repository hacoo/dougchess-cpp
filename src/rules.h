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


class SimpleScoreTable {
 public:
  SimpleScoreTable() {
    scores[0]['k'] = -100000;
    scores[0]['K'] = 100000;
    scores[0]['q'] = -9;
    scores[0]['Q'] = 9;
    scores[0]['r'] = -5;
    scores[0]['R'] = 5;
    scores[0]['b'] = -3;
    scores[0]['B'] = 3;
    scores[0]['n'] = -3;
    scores[0]['N'] = 3;
    scores[0]['p'] = -1;
    scores[0]['P'] = 1;
    scores[0]['.'] = 0;

    scores[1]['k'] = 100000;
    scores[1]['K'] = -100000;
    scores[1]['q'] = 9;
    scores[1]['Q'] = -9;
    scores[1]['r'] = 5;
    scores[1]['R'] = -5;
    scores[1]['b'] = 3;
    scores[1]['B'] = -3;
    scores[1]['n'] = 3;
    scores[1]['N'] = -3;
    scores[1]['p'] = 1;
    scores[1]['P'] = -1;
    scores[1]['.'] = 0;
  }
  int scores[2][128];
};

// Scoring array. Acts as a global lookup table assosciating a piece
// to a score.
static SimpleScoreTable simple_score_table;

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
  Move(Square start, Square finish) : start(start), finish(finish) {}

  Move (const Move& other) : start(other.start),
    finish(other.finish) {}

  // construct from string
  Move(const std::string& m){
    std::vector<std::string> splits = split(m, '-');
    start = Square(splits[0]);
    finish = Square(splits[1]);
  }

  ~Move() {}

  std::string toString() const {
    return start.toString() + "-" + finish.toString() + "\n";
  }

  Square start;
  Square finish;
};




#endif






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

class SimpleScoreTable {
 public:
  SimpleScoreTable() {
    scores[0]['k'] = -100000.0;
    scores[0]['K'] = 100000.0;
    scores[0]['q'] = -9.0;
    scores[0]['Q'] = 9.0;
    scores[0]['r'] = -5.0;
    scores[0]['R'] = 5.0;
    scores[0]['n'] = -3.0;
    scores[0]['N'] = 3.0;
    scores[0]['p'] = -1.0;
    scores[0]['P'] = 1.0;
    scores[0]['.'] = 0.0;

    scores[1]['k'] = 100000.0;
    scores[1]['K'] = -100000.0;
    scores[1]['q'] = 9.0;
    scores[1]['Q'] = -9.0;
    scores[1]['r'] = 5.0;
    scores[1]['R'] = -5.0;
    scores[1]['n'] = 3.0;
    scores[1]['N'] = -3.0;
    scores[1]['p'] = 1.0;
    scores[1]['P'] = -1.0;
    scores[1]['.'] = 0.0;
  }
  float scores[2][128];
};

// Scoring array. Acts as a global lookup table assosciating a piece
// to a score.
static SimpleScoreTable simple_score_table;

#define RANKS 6
#define FILES 5
#define DUMPSIZE 41
#define isValid(x, y) ((x > -1) && (x < FILES) && (y > -1) && (y < RANKS))
#define rankLookup(r) r + 48
#define fileLookup(f) f + 97 
#define ownp(piece, owner) ((owner == 'W') ? isupper(piece) : islower(piece))
#define enemyp(piece, owner) ((owner == 'W') ? islower(piece) : isupper(piece))
#define emptyp(piece) (piece == '.')




// Represents a particular board square
class Square {
 public:
  Square(int rank, int file) : rank(rank), file(file) {}
  Square(const Square& other) : rank(other.rank), file(other.file) {}
  ~Square() {}
  std::string toString() {
    char temp[3] = { (char) fileLookup(file),
		     (char) rankLookup(rank), 
		     '\0' };
    return std::string(temp);
  }

 private:
  int rank;
  int file;
};


// Represents a move from one square to another
class Move {
 public:
  Move(Square start, Square finish) : start(start), finish(finish) {}
  ~Move() {}
  std::string toString() {
    return start.toString() + "-" + finish.toString();
  }
  
 private:
  Square start;
  Square finish;
};




#endif






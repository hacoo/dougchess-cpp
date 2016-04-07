// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 3 Apr. 2016
// 
// dougchess/Boards.h
// 
// Minichess Board. Represents a configuration of the game board
// (including turn number and who is playing)

#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <stdio.h>
#include <cstring>
#include <sstream>
#include <vector>
#include <numeric>
#include "rules.h"
#include "utility.h"
#include "Eval.h"

class Board {

public:
  Board();
  ~Board();
  void reset();
  char winner() const;
  void boardSet(const std::string& b);
  std::string toString() const;
  int getTurn() const;
  char getPlayer() const;
  std::vector<std::string> moves() const;
  std::vector<std::string> movesShuffled() const;
  std::vector<std::string> movesEvaluated() const;
  bool isEnemy(char piece) const;
  bool isOwn(char piece) const;
  bool isNothing(char piece) const;
  int eval() const;
  void move(const std::string& m);
  void undo();
  std::string moveRandom();
  std::string moveGreedy();
  std::string moveNegamax();
  std::string moveAlphabeta();
  std::string repr() const;

private:
  char board[RANKS][FILES];
  int turn;
  char player;
};

#endif 


// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 3 Apr. 2016
// 
// dougchess/Game.h
// 
// Minichess game. Represents a game in progress.

#ifndef GAME_H
#define GAME_H

#include <string>
#include <stdio.h>
#include <cstring>
#include <vector>
#include "rules.h"
#include "Board.h"

class Game {

public:
  Game();
  ~Game();
  void reset();
  void boardSet(const std::string& b);
  int getTurn() const;
  char getPlayer() const;
  std::string mainBoardString() const;
  char winner() const;
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

private:
  Board main_board; // The current game board
  

};

#endif 


// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 3 Apr. 2016
// 
// dougchess/Boards.h
// 
// Minichess Board. Represents a configuration of the game board
// (including turn number and who is playing)

#ifndef BOARD_H
#define BOARD_H

class Board;

#include <string>
#include <stdio.h>
#include <cstring>
#include <sstream>
#include <vector>
#include <numeric>
#include <stack>
#include <algorithm>
#include <random>
#include <time.h>
#include <chrono>
#include "rules.h"
#include "utility.h"
#include "SimpleEval.h"
#include "Movegen.h"
#include "negamax.h"
#include "alphabeta.h"



class Board {

public:
  Board();
  ~Board();
  Board(const Board& other);
  void reset();
  char winner() const;
  void boardSet(const std::string& b);
  std::string toString() const;
  int getTurn() const;
  char getPlayer() const;
  std::vector<Move> moves() const;
  std::vector<Move> movesShuffled() const;
  std::vector<Move> movesEvaluated();
  bool isEnemy(char piece) const;
  bool isOwn(char piece) const;
  bool isNothing(char piece) const;
  int eval() const;
  void move(const std::string& m);
  void move(const Move& m);
  void undo();
  std::string moveRandom();
  std::string moveGreedy();
  std::string moveNegamax(int depth, int duration);
  std::string moveAlphabeta(int depth, int duration);
  std::string repr() const;

private:
  char board[RANKS][FILES];
  int turn;
  char player;
  SimpleEval evaluator;
  Movegen mgen; // Modular move generator
  
  std::stack<Move> undo_move; // last undo move
  std::stack<char> undo_piece; // piece present under undo space
  std::vector<char> pawn_promoted; // track whether a pawn promotion occured
  std::default_random_engine* engine;

  // Functor for move comparison
  struct moveCompare {
    moveCompare(Board* b) : board(b) {}
    Board* board;
    
    bool operator() (const Move& m1, const Move& m2) {
      int eval1;
      int eval2;
      board->move(m1);
      eval1 = -board->eval();
      board->undo();

      board->move(m2);
      eval2 = -board->eval();
      board->undo();
  
      if (eval1 > eval2)
	return true;
      else
	return false;
    }
  };
};

#endif 


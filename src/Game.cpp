// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 3 Apr. 2016
// 
// dougchess/Game.cpp
// 
// Minichess game. Represents a game in progress.
//
// main_board is the current state of the game. However,
// in the long run, this class will probably hold several other
// boards as well, representing different search paths. For now,
// it mostly acts as a wrapper for the main board.

#include"Game.h"

using namespace std;

// Default constructor, start a completely new game.
Game::Game() {
  main_board.reset();
}

Game::~Game() {

}

// Reset the main board to turn 1, starting positions
void Game::reset() {
  main_board.reset();
}

// Set the main board to the contents of the string b
void Game::boardSet(const string& b) {
  main_board.boardSet(b);
}

int Game::getTurn() const {
  return main_board.getTurn();
}

char Game::getPlayer() const {
  return main_board.getPlayer();
}

string Game::mainBoardString() const {
  return main_board.toString();
}

// Determine who the winner is an return it. 
// If no winner yet, return '?'
char Game::winner() const {
  return main_board.winner();
}

vector<string> Game::moves() const {
  return main_board.moves();
}

vector<string> Game::movesShuffled() const {
  return main_board.movesShuffled();
}

vector<string> Game::movesEvaluated() const {
  return main_board.movesEvaluated();
}

bool Game::isEnemy(char piece) const {
  return main_board.isEnemy(piece);
}

bool Game::isOwn(char piece) const {
  return main_board.isOwn(piece);
}


bool Game::isNothing(char piece) const {
  return main_board.isNothing(piece);
}

int Game::eval() const {
  return main_board.eval();
}

// Make requested move. Modifies board state.
void Game::move(const string& m) {
  main_board.move(m);
}

// Undo last move. Modifies board state.
void Game::undo() {
  main_board.undo();
}

// Make a random move, return the move made.
// Will modify the board state.
string Game::moveRandom() {
  return main_board.moveRandom();
}

// Make a greedy move, return the move made.
// Will modify the board state.
string Game::moveGreedy() {
  return main_board.moveGreedy();
}

// Make a negamax move, return the move made.
// Will modify the board state.
string Game::moveNegamax() {
  return main_board.moveNegamax();
}

// Make an alpha-beta move, return the move made.
// Will modify the board state.
string Game::moveAlphabeta() {
  return main_board.moveAlphabeta();
}

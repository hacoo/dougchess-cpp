// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 3 Apr. 2016
// 
// dougchess/Boards.h
// 
// Minichess Board. Represents a configuration of the game board
// (including turn number and who is playing)


#include "Board.h"

using namespace std;

// Default constructor, initialize to an empty board
Board::Board() {
  for (int y = 0; y < RANKS; ++y) {
      for (int x = 0; x < FILES; ++x)
      board[y][x] = '.';
  }
  turn = 0;
  player = 'W';
}

Board::~Board() {

}


string Board::toString() const {
  char buf[37];
  for (int y = 0; y < RANKS; ++y) {
    for (int x = 0; x < FILES; ++x) {
      buf[y*(FILES+1) + x] = board[y][x];
    }
  }
   
  for (int y = 0; y < RANKS; ++y) {
    buf[y*(FILES+1) + 5] = '\n';
  }
   
  buf[36] = '\0';

  ostringstream os;
  os << turn << " " << player << '\n' << buf;
  return os.str();
}

// Return the winner, or '?' if the game is not yet decided.
// Will return a draw if both kings die, or the time runs out.
char Board::winner() const {
  if (turn >= 41)
    return '=';
  
  bool white_alive = (find_piece(board, 'K') != -1);
  bool black_alive = (find_piece(board, 'k') != -1);
  
  if (white_alive && black_alive)
    return '?';
  if (white_alive)
    return 'W';
  if (black_alive)
    return 'B';
  cout << "ERROR -- both kings are dead! Chaos descends on the land...\n";
  cout << "board state: \n";
  cout << repr();
  return '=';
} 

// Reset to starting game state
void Board::reset() {
  turn = 1;
  player = 'W';
  char b [RANKS][FILES] = {{'k', 'q', 'b', 'n', 'r'},
			   {'p', 'p', 'p', 'p', 'p'},
			   {'.', '.', '.', '.', '.'},
			   {'.', '.', '.', '.', '.'},
			   {'P', 'P', 'P', 'P', 'P'},
			   {'R', 'N', 'B', 'Q', 'K'}};
  for (int y = 0; y < RANKS; ++y) {
    for (int x = 0; x < FILES; ++x) {
      board[y][x] = b[y][x];
    }
  }
}

// Set the board to the contents of b.
void Board::boardSet(const string& b) {
  parse_board(b, &turn, &player, board);
  cout << repr();  
}

// Get all available moves for the current turn
vector<string> Board::moves() const {
  vector<string> ms;

  ms.push_back("a2-a3\n");
  ms.push_back("b2-b3\n");
  ms.push_back("c2-c3\n");
  ms.push_back("d2-d3\n");
  ms.push_back("e2-e3\n");
  ms.push_back("b1-a3\n");
  ms.push_back("b1-c3\n");
  
  return ms;
}

// Return moves in random order
vector<string> Board::movesShuffled() const {
  return moves();
}

// Return moves in order of evaluation
vector<string> Board::movesEvaluated() const {
  return moves();
}

char Board::getPlayer() const {
  return player;
}

int Board::getTurn() const {
  return turn;
}

bool Board::isEnemy(char piece) const {
  if (piece == '.')
    return false;
  bool uppercase = isupper(piece);
  if (player == 'W')
    return !uppercase;
  else
    return uppercase;
}

bool Board::isOwn(char piece) const {
  if (piece == '.')
    return false;
  bool uppercase = isupper(piece);
  if (player == 'W')
    return uppercase;
  else
    return !uppercase;

}

bool Board::isNothing(char piece) const {
  return (piece == '.');
}

int Board::eval() const {
  return 0;
}


// Make the requested move, will modify board state.
void Board::move(const string& m) {
  return;
}

// Undo the last move. Will modify board state
void Board::undo() {
  return;
}


// Make a random move, return the move made.
// Will modify the board state.
string Board::moveRandom() {
  return string("a5-a4\n");
}

// Make a greedy move, return the move made.
// Will modify the board state.
string Board::moveGreedy() {
  return string("a5-a4\n");
}

// Make a negamax move, return the move made.
// Will modify the board state.
string Board::moveNegamax() {
  return string("a5-a4\n");
}

// Make an alpha-beta move, return the move made.
// Will modify the board state.
string Board::moveAlphabeta() {
  return string("a5-a4\n");
}

// Make string representation of the board
string Board::repr() const {
  ostringstream os;
  os << turn << " " << player << "\n" << board_to_string(board);
  return os.str();
}

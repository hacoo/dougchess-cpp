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
  if (turn > 41)
    return '=';

  bool white_alive = (find_piece(board, 'K') != -1);
  bool black_alive = (find_piece(board, 'k') != -1);

  if (turn == 41) {
    if (player == 'W' && black_alive && !white_alive) 
      return 'B';
    else
      return '=';
  }
  
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
}

// Get all available moves for the current player in 
// this board configuration. Return as a vector of strings.
vector<Move> Board::moves() const {
  vector<Move> ms;
  mgen.generateMoves(board, player, ms);
  return ms;
}


// Return moves in random order
vector<Move> Board::movesShuffled() const {
  return moves();
}

// Return moves in order of evaluation
vector<Move> Board::movesEvaluated() const {
  return moves();
}

char Board::getPlayer() const {
  return player;
}

int Board::getTurn() const {
  return turn;
}

bool Board::isEnemy(char piece) const {
  return enemyp(piece, player);
}

bool Board::isOwn(char piece) const {
  return ownp(piece, player);
}

bool Board::isNothing(char piece) const {
  return emptyp(piece);
}

int Board::eval() const {
  return Eval::score_simple(board, player);
}


// Make the requested move, will modify board state.
void Board::move(const string& m) {

  move(Move(m));
  return;
}

// Make the requested move, will modify board state.
// Also performs sanity check and will crash on an 
// invalid mode.
void Board::move(const Move& m) {
  
  int srank = m.start.rank;
  int frank = m.finish.rank;
  int sfile = m.start.file;
  int ffile = m.finish.file;

  // Do a sanity check! Does NOT verify that the move
  // was legal for the piece.
  if (!isValid(srank, sfile) ||
       !isValid(frank, ffile) || 
      !ownp(board[srank][sfile], player) ||
      ownp(board[frank][ffile], player))
    {
    cout << "ERROR -- tried to make invalid move\n" 
  	 << " Move: " << m.toString() << "\n"
  	 << " Board state:\n" 
  	 << repr() << endl;
    cout << "The indices were: \n"
  	 << " start rank: " << srank << "\n"
      	 << " start file: " << sfile << "\n"
      	 << " end rank: " << frank << "\n"
      	 << " end file: " << ffile << "\n";
    cout << "start valid: " << isValid(srank, sfile) << "\n"
  	 << "end valid: " << isValid(frank, ffile) << "\n"
  	 << "start own: " << ownp(board[srank][sfile], player) << "\n"
      	 << "end own: " << ownp(board[frank][ffile], player) << "\n";
    exit(1);
  }

  // make the move
  char piece = board[srank][sfile];
  // handle queen promotion
  if ((piece == 'p') && (frank == RANKS-1)) {
    board[frank][ffile] = 'q';
  }
  else if ((piece == 'P') && (frank == 0)) {
    board[frank][ffile] = 'Q';
  }
  else {
    board[frank][ffile] = piece;
  }

  board[srank][sfile] = '.';

  // switch who is playing
  if (player == 'W') {
    player = 'B';
  } else {
    player = 'W';
    ++turn; // increment every other turn
  }

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

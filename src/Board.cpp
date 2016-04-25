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
Board::Board(TimeManager& manager) : manager(manager) {
  for (int y = 0; y < RANKS; ++y) {
      for (int x = 0; x < FILES; ++x)
      board[y][x] = '.';
  }

  engine = new default_random_engine(time(NULL));
  
  turn = 0;
  player = 'W';
}

// Copy constructor -- does NOT copy undo history
Board::Board(const Board& other) 
  : turn(other.turn), 
    player(other.player),
    manager(manager) {
  for (int y = 0; y < RANKS; ++y) {
    for (int x = 0; x < FILES; ++x) {
      board[y][x] = other.board[y][x];
    }
  }
  engine = new default_random_engine(time(NULL));
}
  
Board::~Board() {
  delete engine;
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
  if (turn > MAX_TURNS + 1)
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
  // char b [RANKS][FILES] = {{'.', '.', '.', '.', '.'},
  //  			   {'.', 'P', '.', '.', '.'},
  // 			   {'.', '.', 'p', '.', '.'},
  // 			   {'.', '.', '.', 'P', '.'},
  // 			   {'.', 'p', '.', 'k', 'K'},
  // 			   {'.', '.', '.', '.', '.'}};

  
  for (int y = 0; y < RANKS; ++y) {
    for (int x = 0; x < FILES; ++x) {
      board[y][x] = b[y][x];
    }
  }

  // clear undo stacks
  while (!undo_move.empty())
    undo_move.pop();
  while (!undo_piece.empty())
    undo_piece.pop();
  pawn_promoted.clear();
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
  vector<Move> ms;
  mgen.generateMoves(board, player, ms);
  shuffle(begin(ms), end(ms), *engine);
  return ms;
}

// Return moves in order of evaluation
vector<Move> Board::movesEvaluated() {
  vector<Move> ms;
  mgen.generateMoves(board, player, ms);
  // shuffle to randomize 'equal' moves
  shuffle(begin(ms), end(ms), *engine); 
  sort(ms.begin(), ms.end(), moveCompare(this));
  return ms;
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
  return evaluator.eval(board, player);
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
    cout << "ERROR -- tried to make invalid undo\n" 
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
  
  // catch undo piece before it's overwritten'
  char piece = board[srank][sfile];
  undo_piece.push(board[frank][ffile]);

  // Make the move
  if ((piece == 'p') && (frank == RANKS-1)) {
    pawn_promoted.push_back('T');
    board[frank][ffile] = 'q';
  }
  else if ((piece == 'P') && (frank == 0)) {
    pawn_promoted.push_back('T');
    board[frank][ffile] = 'Q';
  }
  else {
    pawn_promoted.push_back('F');
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

  // set the undo move
  undo_move.push(Move(m.finish, m.start));
 
  return;
}

// Undo the last move. Will modify board state
void Board::undo() {
  
  if (undo_move.empty()) {
    cout << "ERROR -- no move to undo" << endl;
    return;
  }

  if (undo_piece.empty()) {
    cout << "ERROR -- no piece to undo. This should not happen!" << endl;
    exit(1);
    return;
  }
  
  Move m = undo_move.top();
  char p = undo_piece.top();
  char promotion = pawn_promoted.back();
  undo_move.pop();
  undo_piece.pop();
  pawn_promoted.pop_back();

  int srank = m.start.rank;
  int frank = m.finish.rank;
  int sfile = m.start.file;
  int ffile = m.finish.file;

  // Do a sanity check! Does NOT verify that the move
  // was legal for the piece.
  if (!isValid(srank, sfile) ||
      !isValid(frank, ffile) || 
      !enemyp(board[srank][sfile], player) ||
      !emptyp(board[frank][ffile]))
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

  // Check for pawn promotion
  if (promotion == 'T') {
    if (player == 'B')
      board[srank][sfile] = 'P';
    else
      board[srank][sfile] = 'p';
  }
  
  // move the piece back
  board[frank][ffile] = board[srank][sfile];
  // restore the previous piece
  board[srank][sfile] = p;
  
  // switch who is playing
  if (player == 'B') {
    player = 'W';
  } else {
    player = 'B';
    --turn; // decrement every other turn
  }

  return;
}

// Make a random move, return the move made.
// Will modify the board state.
string Board::moveRandom() {
  vector<Move> ms = movesShuffled();
  move(ms[0]);
  return ms[0].toString();
}

// Make a greedy move, return the move made.
// Will modify the board state.
// Makes the move that will result in the highest
// board evaluation, one move ahead.
string Board::moveGreedy() {
  vector<Move> ms = movesEvaluated();
  move(ms[0]);
  return ms[0].toString();
}

// Make a negamax move, return the move made.
// Will modify the board state.
string Board::moveNegamax(int depth, int duration) {
  Move m = negamax_move(*this, depth);
  move(m);
  return m.toString();
}

// Make an alpha-beta move, return the move made.
// Will modify the board state.
string Board::moveAlphabeta(int depth, int duration) {
  //ProfilerStart("alphabeta.log");
  Move m = alphabeta_move(*this, depth);
				          
  //cout << repr() << endl;

  move(m);
  //ProfilerStop();
  
  //cout << repr() << endl;
  //cout << "Returning: " << m.toString() << endl;

  return m.toString();
}

// Make string representation of the board
string Board::repr() const {
  ostringstream os;
  os << "  " << turn << " " << player << "\n" 
     << board_to_string(board) << "\n"
     << "  Eval:   " << eval() << "\n"
     << "  Winner: " << winner() << "\n";
    
  return os.str();
}

// Fill board state into b
void Board::getBoard(char b[RANKS][FILES]) const {
  for (int y = 0; y < RANKS; ++y) {
    for (int x = 0; x < FILES; ++x) {
      b[y][x] = board[y][x];
    }
  }
}

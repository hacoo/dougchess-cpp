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
Board::Board(TimeManager& manager,
	     ZobristTable& zobrist,
	     TranspositionTable& tt) : turn(0),
				       player('W'),
				       manager(manager),
				       zobrist(zobrist),
				       tt(tt),
				       score(0) {

  for (int y = 0; y < RANKS; ++y) {
      for (int x = 0; x < FILES; ++x)
      board[y][x] = '.';
  }

  init_scoretable();
  init_pawnpushtable();
  init_boardmask();
  
  engine = new default_random_engine(time(NULL)); 
}

// Copy constructor -- does NOT copy undo history
Board::Board(const Board& other) 
  : turn(other.turn), 
    player(other.player),
    manager(manager),
    zobrist(other.zobrist),
    tt(other.tt),
    score(other.score) {
  
  for (int y = 0; y < RANKS; ++y) {
    for (int x = 0; x < FILES; ++x) {
      board[y][x] = other.board[y][x];
    }
  }

  init_scoretable();
  init_pawnpushtable();
  init_boardmask();
  
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
  score = 0;

  currently_pondering_atom = false;
  continue_pondering_atom = false;
  
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

// Clear the board, making all spaces empty
void Board::clear() {
  char b [RANKS][FILES] = {{'.', '.', '.', '.', '.'},
   			   {'.', '.', '.', '.', '.'},
  			   {'.', '.', '.', '.', '.'},
  			   {'.', '.', '.', '.', '.'},
  			   {'.', '.', '.', '.', '.'},
  			   {'.', '.', '.', '.', '.'}};

  for (int y = 0; y < RANKS; ++y) {
    for (int x = 0; x < FILES; ++x) {
      board[y][x] = b[y][x];
    }
  }

  score = 0;
  
  // clear undo stacks
  while (!undo_move.empty())
    undo_move.pop();
  while (!undo_piece.empty())
    undo_piece.pop();
  pawn_promoted.clear();  
}

void Board::setPieceAt(char piece, int rank, int file) {
  board[rank][file] = piece;
}

void Board::setPlayer(char _player) {
  player = _player;
}

// Set the board to the contents of b.
void Board::boardSet(const string& b) {
  parse_board(b, &turn, &player, board);
  
  score = 0;
  // update score to reflect new board
  for (int y = 0; y < RANKS; ++y) { 
    for (int x = 0; x < FILES; ++x) {
	score += pieceScore(y, x);
      }
  }
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

// Use TT scroe to order moves if available.
// If not, fall back to a greedy move.
vector<Move> Board::movesTT() {
  vector<Move> ms;
  u64 hash = zobristHash();
  mgen.generateMoves(board, player, ms);
  // shuffle to randomize 'equal' moves
  shuffle(begin(ms), end(ms), *engine); 
  sort(ms.begin(), ms.end(), moveCompareTT(this, &tt, hash));
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
  // Old -- modular evaluator
  //return evaluator.eval(board, player);

  // Just return incremental eval
  if (player == 'W')
    return score;
  else
    return -score;
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
  /* DISABLED for tournament play
  if (!isValid(srank, sfile) ||
       !isValid(frank, ffile) || 
      !ownp(board[srank][sfile], player) ||
      ownp(board[frank][ffile], player)) {
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
  */
  
  // catch undo piece before it's overwritten'
  char piece = board[srank][sfile];
  undo_piece.push(board[frank][ffile]);
  
  // update score to reflect captured piece
  score -= pieceScore(frank, ffile);
  // update score to take into account moved piece
  score -= pieceScore(srank, sfile);
  
  
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

  // update score to reflect position of moved piece
  score += pieceScore(frank, ffile);

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
  /*
  if (undo_move.empty()) {
    cout << "ERROR -- no move to undo" << endl;
    return;
  }

  if (undo_piece.empty()) {
    cout << "ERROR -- no piece to undo. This should not happen!" << endl;
    exit(1);
    return;
  }
  */
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
  /* DISABLED for tournament play
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
  */

  // remove score of piece at starting position
  score -= pieceScore(srank, sfile);
  
  // Check for pawn promotion
  if (promotion == 'T') {
    if (player == 'B') {
      board[srank][sfile] = 'P';
    }
    else {
      board[srank][sfile] = 'p';
    } 
  }
  
  // move the piece back
  board[frank][ffile] = board[srank][sfile];
  // restore the previous piece
  board[srank][sfile] = p;

  // add in score for moved piece at new location
  score += pieceScore(frank, ffile);
  // add in score for previously captured piece
  score += pieceScore(srank, sfile);
  
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
  
  //stopPondering();
  //ProfilerStart("alphabeta.log");
  CALLGRIND_START_INSTRUMENTATION;
  cout << "Duration: " << duration << endl;
  Move searching;
  Move m = movesShuffled()[0];
  // If depth is -1, we are in tournament mode
  bool tournamentMode = (depth == -1) ? true : false;

  // If in tournament mode, start the timer. Search
  // will be controlled by timer, or to a max depth of 8.
  if (tournamentMode) {
    manager.start(*this);
    depth = 20;
  }

  chrono::milliseconds start;
  chrono::milliseconds stop;

  int i = 1;
  
  try { 
    // use iterative deepening
    while (i <= depth) {
      cout << "Beginning search at depth: " << i << "\n";
      // Only search past 7 if there's more than half time remaining
      if (depth > 7) {
	if (manager.early_out_of_time()) {
	  cout << "ABORTED -- Not enough time to continue" << endl;
	  break;
	}
      }
      start = ms_now();
      searching = alphabeta_move(*this, i, manager, zobrist, tt, false);
      stop  = ms_now();
      m.clone(searching);
      cout << "Searched to depth: " << i << "\n"
	   << "  Time:            " << (stop-start).count() << "\n"
	   << "  Result:          " << m.toString() << "\n"
	   << "  TT hits:         " << tt.getHits() << "\n"
	   << "  TT misses:       " << tt.getMisses() << "\n"
	   << "  TT replacements: " << tt.getReplacements() << "\n"
	   << "  TT conflicts:    " << tt.getConflicts() << "\n"
	   << "  TT stores:       " << tt.getStores() << endl;
	
      ++i;
    }
  } catch(OutOfTimeException& e) {
    cout << "Timer ran out at search depth: " << i << endl;
  }

  if (tournamentMode)
    manager.stop();

  cout << "Making move: " << m.toString() << endl;
  move(m);


  CALLGRIND_STOP_INSTRUMENTATION;
  CALLGRIND_DUMP_STATS;
  //ProfilerStop();

  /*
  if(tournamentMode) {
    thread ponderer([this]() {
	this->startPondering();
    });
    ponderer.detach();
  }
  */
  
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

// Return the Zobrist hash for this board
u64 Board::zobristHash() const {
  return zobrist.hash_board(board, player);
}

// Return the updated that will result AFTER move move is made.
u64 Board::updateHash(const u64 old, const Move& move) const {
  return zobrist.update_hash(old, board, player, move);
}

// Return the value of the piece at position [y][x],
// assuming White's perpsective
int Board::pieceScore(const int y, const int x) const {
  char piece = board[y][x];
  int s = scoretable[piece] + pawnpushtable[piece][y];
  //int s = scoretable[piece] + boardmask[piece][y][x];
  // if (piece == 'p')
  //   return s - (y * PAWN_PUSH_VALUE);
  // if (piece == 'P')
  //   return s + (RANKS - 1 - y) * PAWN_PUSH_VALUE;
  return s;
}


void Board::init_scoretable() {
  scoretable['.'] = 0;

  scoretable['p'] = -100;
  scoretable['n'] = -300;
  scoretable['b'] = -300;
  scoretable['r'] = -500;
  scoretable['q'] = -900;
  scoretable['k'] = -1000000;

  scoretable['P'] = 100;
  scoretable['N'] = 300;
  scoretable['B'] = 300;
  scoretable['R'] = 500;
  scoretable['Q'] = 900;
  scoretable['K'] = 1000000;

}


void Board::init_pawnpushtable() {
  for (int piece = 0; piece < 128; ++piece) {
    for(int y = 0; y < RANKS; ++y) {
      if (piece == 'p')
	pawnpushtable[piece][y] = -(y*PAWN_PUSH_VALUE);
      else if (piece == 'P')
	pawnpushtable[piece][y] = (RANKS - 1 - y) * PAWN_PUSH_VALUE;
      else
	pawnpushtable[piece][y] = 0;
    }
  }  
}

// pawn push / center control boardmask
void Board::init_boardmask() {
  for (int piece = 0; piece < 128; ++piece) {
    for(int y = 0; y < RANKS; ++y) {
      for (int x = 0; x < FILES; ++x) {
	if (piece == '.')
	  boardmask[piece][y][x] = 0;
	else if (piece == 'P')
	  boardmask[piece][y][x] = (RANKS - 1 - y) * PAWN_PUSH_VALUE;
	else if (piece == 'p')
	  boardmask[piece][y][x] = -(y*PAWN_PUSH_VALUE);
	else if(isupper(piece)) {
	  boardmask[piece][y][x] = 10 - 5 * abs(2-x);
	  if (y > 2) 
	    boardmask[piece][y][x] += 15 - 5 * abs(2-y);
	  else
	    boardmask[piece][y][x] += 15 - 5 * abs(3-y);
	}
	else if(islower(piece)) {
	  boardmask[piece][y][x] = -(10 - 5 * abs(2-x));
	  if (y > 2) 
	    boardmask[piece][y][x] -= 15 - 5 * abs(2-y);
	  else
	    boardmask[piece][y][x]-= 15 - 5 * abs(3-y);
	}
	else
	  boardmask[piece][y][x] = 0;
      }
    }
  }
}



void Board::startPondering() {
  currently_pondering_atom.store(true);
  continue_pondering_atom.store(true);
  cout << "Starting to ponder... " << endl;

  Move searching;
  Move m = movesShuffled()[0];
  int i = 1;
    while (true) {
      try {
	cout << "Pondering depth: " << i << endl;
	m = alphabeta_move(*this, i, manager, zobrist, tt, true);
	++i;
      }

      catch(PonderDoneException& e) {
	cout << "Pondering interrupted. " << endl;
	break;
      }
      catch(OutOfTimeException& e) {
	cout << "A timer went off, who cares" << endl;
      }
    }
  currently_pondering_atom.store(false); // Signal that pondering is done
}


void Board::stopPondering() {
  continue_pondering_atom.store(false);
  if(!currently_pondering_atom.load()) {
    cout << "WARNING -- tried to stop pondering, but we're not pondering right now " << endl;
    return;
  }
  while(currently_pondering_atom.load())
    ; // Spin wait until pondering wraps up
}


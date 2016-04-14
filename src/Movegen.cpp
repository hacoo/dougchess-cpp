// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 13 Apr. 2016
// 
// dougchess/movegen.cpp
// 
// Move generation for dougchess.

#include "Movegen.h"
using namespace std;

Movegen::Movegen() {
}

Movegen::~Movegen() {
}


// Generate all moves and add them into vector ms.
void Movegen::generateMoves(const char board[RANKS][FILES],
			    char player,
			    vector<Move>& ms) {
  for (int y = 0; y < RANKS; ++y) {
    for (int x = 0; x < FILES; ++x) {
      if(ownp(board[y][x], player))
	insertPieceMove(board, player, y, x, ms);
    }
  }
}


// Insert move for piece at x, y
void Movegen::insertPieceMove(const char board[RANKS][FILES],
			      char player,
			      int rank,
			      int file,
			      vector<Move>& ms) {
  cout << "Generating: " << rank << " " << file << endl;
  switch(board[rank][file]) {
    
  case 'k' : insertKingMoves(board, player, rank, file, ms);
             cout << "Generating king moves" << endl;
	     break;
  case 'K' : insertKingMoves(board, player, rank, file, ms);
             cout << "Generating king moves" << endl;
	     break;
  case 'q' : break;
  case 'Q' : break;
  case 'r' : insertRookMoves(board, player, rank, file, ms);
             cout << "Generating rook moves" << endl;
	     break;
  case 'R' : insertRookMoves(board, player, rank, file, ms);
             cout << "Generating rook moves" << endl;
	     break;

  case 'n' : insertKnightMoves(board, player, rank, file, ms);
             cout << "Generating knight moves" << endl;
	     break;
  case 'N' : insertKnightMoves(board, player, rank, file, ms);
             cout << "Generating knight moves" << endl;
	     break;
  case 'b' : break;
  case 'B' : break;
  case 'p' : insertPawnMoves(board, player, rank, file, ms);
             cout << "Generating pawn moves" << endl;
	     break;
  case 'P' : insertPawnMoves(board, player, rank, file, ms);
             cout << "Generating pawn moves" << endl;
	     break;
  case '.' : break;
  default  : {
    cout << "ERROR - invalid piece " << board[rank][file] << endl;
    exit(1);
  }   
  }
}

// Insert king moves for piece at [rank][file] into ms
void Movegen::insertKingMoves(const char board[RANKS][FILES],
			      char player,
			      int rank,
			      int file,
			      vector<Move>& ms) {

  sanityCheck(board, player, 'k', rank, file);
  
  Square start(rank, file);
  
  if((isValid(rank-1, file-1)) &&
     (!ownp(board[rank-1][file-1], player)))
    ms.push_back(Move(start, Square(rank-1, file-1)));

  if((isValid(rank-1, file)) &&
     (!ownp(board[rank-1][file], player)))
    ms.push_back(Move(start, Square(rank-1, file)));
  
  if((isValid(rank-1, file+1)) &&
     (!ownp(board[rank-1][file+1], player)))
    ms.push_back(Move(start, Square(rank-1, file+1)));

  if((isValid(rank, file-1)) &&
     (!ownp(board[rank][file-1], player)))
    ms.push_back(Move(start, Square(rank, file-1)));

  if((isValid(rank, file+1)) &&
     (!ownp(board[rank][file+1], player)))
    ms.push_back(Move(start, Square(rank, file+1)));

  if((isValid(rank+1, file-1)) &&
     (!ownp(board[rank+1][file-1], player)))
    ms.push_back(Move(start, Square(rank+1, file-1)));

  if((isValid(rank+1, file)) &&
     (!ownp(board[rank+1][file], player)))
    ms.push_back(Move(start, Square(rank+1, file)));

  if((isValid(rank+1, file+1)) &&
     (!ownp(board[rank+1][file+1], player)))
    ms.push_back(Move(start, Square(rank+1, file+1)));
}

void Movegen::insertPawnMoves(const char board[RANKS][FILES],
			      char player,
			      int rank,
			      int file,
			      vector<Move>& ms) {
 
  sanityCheck(board, player, 'p', rank, file);
 
  Square start(rank, file);
  
  // if black, you will move up the board. If white, you move down.

  if (player == 'B') {
    if((isValid(rank+1, file)) &&
       (!ownp(board[rank+1][file], player)))
      ms.push_back(Move(start, Square(rank+1, file)));

    // Diagonal capture moves
    if((isValid(rank+1, file-1)) &&
       (enemyp(board[rank+1][file-1], player)))
      ms.push_back(Move(start, Square(rank+1, file-1)));

    if((isValid(rank+1, file+1)) &&
       (enemyp(board[rank+1][file+1], player)))
      ms.push_back(Move(start, Square(rank+1, file+1)));

  } else {

    if((isValid(rank-1, file)) &&
       (!ownp(board[rank-1][file], player)))
      ms.push_back(Move(start, Square(rank-1, file)));

    // Diagonal capture moves
    if((isValid(rank-1, file-1)) &&
       (enemyp(board[rank-1][file-1], player)))
      ms.push_back(Move(start, Square(rank+1, file-1)));

    if((isValid(rank-1, file+1)) &&
       (enemyp(board[rank-1][file+1], player)))
      ms.push_back(Move(start, Square(rank-1, file+1)));

  }  
}

void Movegen::insertKnightMoves(const char board[RANKS][FILES],
				char player,
				int rank,
				int file,
				vector<Move>& ms) {

  sanityCheck(board, player, 'n', rank, file);
 
  Square start(rank, file);

  if((isValid(rank+2, file-1)) &&
     (!ownp(board[rank+2][file-1], player)))
    ms.push_back(Move(start, Square(rank+2, file-1)));

  if((isValid(rank+2, file+1)) &&
     (!ownp(board[rank+2][file+1], player)))
    ms.push_back(Move(start, Square(rank+2, file+1)));

  if((isValid(rank-2, file-1)) &&
     (!ownp(board[rank-2][file-1], player)))
    ms.push_back(Move(start, Square(rank-2, file-1)));

  if((isValid(rank-2, file+1)) &&
     (!ownp(board[rank-2][file+1], player)))
    ms.push_back(Move(start, Square(rank-2, file+1)));

  if((isValid(rank-1, file+2)) &&
     (!ownp(board[rank-1][file+2], player)))
    ms.push_back(Move(start, Square(rank-1, file+2)));
  
  if((isValid(rank+1, file+2)) &&
     (!ownp(board[rank+1][file+2], player)))
    ms.push_back(Move(start, Square(rank+1, file+2)));

  if((isValid(rank-1, file-2)) &&
     (!ownp(board[rank-1][file-2], player)))
    ms.push_back(Move(start, Square(rank-1, file-2)));

  if((isValid(rank+1, file-2)) &&
     (!ownp(board[rank+1][file-2], player)))
    ms.push_back(Move(start, Square(rank+1, file-2)));  

}

void Movegen::insertRookMoves(const char board[RANKS][FILES],
				char player,
				int rank,
				int file,
				vector<Move>& ms) {

  sanityCheck(board, player, 'r', rank, file);
 
  Square start(rank, file);
  int r;
  int f;
  
  // down
  r = rank+1;
  f = file;
  while (isValid(r, f) && emptyp(board[r][f])) {
    ms.push_back(Move(start, Square(r, f)));
    ++r;
  }
  if (enemyp(board[r][f], player)) 
    ms.push_back(Move(start, Square(r, f)));
  
  // up 
  r = rank-1;
  while (isValid(r, f) && emptyp(board[r][f])) {
    ms.push_back(Move(start, Square(r, f)));
    --r;
  }
  if (enemyp(board[r][f], player)) 
    ms.push_back(Move(start, Square(r, f)));

  // left
  r = rank;
  f = file-1;
  while (isValid(r, f) && emptyp(board[r][f])) {
    ms.push_back(Move(start, Square(r, f)));
    --f;
  }
  if (enemyp(board[r][f], player)) 
    ms.push_back(Move(start, Square(r, f)));
  
  // right
  f = file+1;
  while (isValid(r, f) && emptyp(board[r][f])) {
    ms.push_back(Move(start, Square(r, f)));
    ++f;
  }
  if (enemyp(board[r][f], player)) 
    ms.push_back(Move(start, Square(r, f)));
}



// Do a sanity check and crash if you try to generate moves
// for the enemy or an invalid square
void Movegen::sanityCheck(const char board[RANKS][FILES],
			  char player,
			  char expected,
			  int rank,
			  int file) {

  if (!isValid(rank, file)) {
    cout << "ERROR -- tried to generate move for invalid index: " 
	 << rank << " " << file << endl;
    exit(1);
  }
  if (!ownp(board[rank][file], player)) {
    cout << "ERROR -- tried to move piece that's not yours at " 
	 << rank << " " << file << " with value " << board[rank][file] 
	 << endl;
    exit(1);
  }
  if (tolower(expected) != tolower(board[rank][file])) {
    cout << "ERROR -- tried to generated moves for wrong piece type.\n"
	 << "Expected: " << expected << "\n"
	 << "Got: " << board[rank][file] << endl;
    exit(1);
  }
}

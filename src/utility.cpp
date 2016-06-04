// 4 Apr. 2016
// 
// dougchess/utility.cpp
// 
// General Minichess utility functions.

#include "utility.h"

using namespace std;


// Parse the board string b. Return the turn in newturn, the 
// player in newplayer, and the board characters in newboard.
// Return 0 on success, 1 on failure.
int parse_board(const std::string b, int* newturn,
		char* newplayer, char newboard[RANKS][FILES]) {
  vector<string> splits = split(b, '\n');

  // set player and turn
  sscanf(splits[0].c_str(), "%d %c", newturn, newplayer);

  for (int y = 0; y < RANKS; ++y) {
    for (int x = 0; x < FILES; ++x) {
      newboard[y][x] = splits[y+1][x];
    }
  }
  
  return 0;
}

void print_string_vec(const vector<string>& v) {
  cout << "[";
  for (vector<string>::const_iterator i = v.begin();
       i != v.end();
       ++i) {
    cout << *i << ", ";
  }
  cout << "]\n";
}

string board_to_string(const char board[RANKS][FILES]) {

  stringstream sstream;
  
  for (int y = 0; y < RANKS; ++y) {
    sstream << 6-y << " ";
    for (int x = 0; x < FILES; ++x) {
      sstream << board[y][x] << " ";
    }
    sstream << "\n";
  }
  sstream << "  a b c d e\n";
  
  return sstream.str();
}


string flat_board_to_string(const char* board) {

  stringstream sstream;
  
  for (int y = 0; y < RANKS; ++y) {
    sstream << 6-y << " ";
    for (int x = 0; x < FILES; ++x) {
      sstream << board[y*FILES + x] << " ";
    }
    sstream << "\n";
  }
  sstream << "  a b c d e\n";
  
  return sstream.str();
}

string lwBoard_to_string(const lwBoard& board) {

  stringstream sstream;
  
  for (int y = 0; y < RANKS; ++y) {
    sstream << 6-y << " ";
    for (int x = 0; x < FILES; ++x) {
      sstream << board.board[y][x] << " ";
    }
    sstream << "\n";
  }
  sstream << "  a b c d e\n";
  
  return sstream.str();
}


// Return the position of a piece on the board. If it's
// not found, return -1.
int find_piece(const char board[RANKS][FILES], char piece) {
  for (int y = 0; y < RANKS; ++y) {
    for (int x = 0; x < FILES; ++x) {
      if (board[y][x] == piece)
	return (y * FILES + x);
    }  
  }
  return -1;
}

string current_time_string()  {

  stringstream sstream;
  
  boost::posix_time::ptime now = boost::posix_time::
    microsec_clock::local_time();

    const boost::posix_time::time_duration td = now.time_of_day();
    const long hours        = td.hours();
    const long minutes      = td.minutes();
    const long seconds      = td.seconds();
    const long milliseconds = td.total_milliseconds() -
                              ((hours * 3600 + minutes * 60 + seconds) *
			       1000);
    char buf[40];
    sprintf(buf, "%02ld:%02ld:%02ld.%03ld", 
        hours, minutes, seconds, milliseconds);

    return string(buf);
}


chrono::milliseconds ms_now() {
  return chrono::duration_cast<chrono::milliseconds>
    (chrono::system_clock::now().time_since_epoch());  
}

void printCharVec(vector<char> v) {
  for (auto i : v) {
    cout << i << endl;
  }
}

char pieceToInt(char piece) {
  switch(piece) {
  case '.': return (char) EMPTY;
  case 'p': return (char) BPAWN;
  case 'n': return (char) BKNIGHT;
  case 'b': return (char) BBISHOP;
  case 'r': return (char) BROOK;
  case 'q': return (char) BQUEEN;
  case 'k': return (char) BKING;
  case 'P': return (char) WPAWN;
  case 'N': return (char) WKNIGHT;
  case 'B': return (char) WBISHOP;
  case 'R': return (char) WROOK;
  case 'Q': return (char) WQUEEN;
  case 'K': return (char) WKING;    
  }
  cout << "ERROR -- unknown piece to integer conversion" << endl;
  return 0;
}

char intToPiece(char i) {
  switch((int) i) {
  case EMPTY: return  '.';
  case BPAWN: return  'p';
  case BKNIGHT: return  'n';
  case BBISHOP: return  'b';
  case BROOK: return  'r';
  case BQUEEN: return  'q';
  case BKING: return  'k';
  case WPAWN: return  'P';
  case WKNIGHT: return  'N';
  case WBISHOP: return  'B';
  case WROOK: return  'R';
  case WQUEEN: return  'Q';
  case WKING: return  'K';
  }
  cout << "ERROR -- unknown integer to piece conversion" << endl;
  return 0;
}

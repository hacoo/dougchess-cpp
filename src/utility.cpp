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
  sscanf(splits[0].c_str(), "%d %c", newturn, newplayer);
  for (int y = 0; y < RANKS; ++y) 
    for (int x = 0; x < FILES; ++x) {
      newboard[y][x] = splits[y+1][x];
    }
  
  return 0;
}

vector<string>& split(const string& s, char delim,
		      vector<string>& elems) {
  stringstream sstream(s);
  string item;
  while(getline(sstream, item, delim)) {
    if(!item.empty())
      elems.push_back(item);
  }
  return elems;
}

vector<string> split(const string& s, char delim) {
  vector<string> elems;
  split(s, delim, elems);
  return elems;
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
    for (int x = 0; x < FILES; ++x) {
      sstream << board[y][x];
    }
    sstream << "\n";
  }
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
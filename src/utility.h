// 4 Apr. 2016
// 
// dougchess/utility.h
// 
// General Minichess utility functions.

#ifndef UTILITY_H
#define UTILITY_H

#include <cstring>
#include <string>
#include <iostream>
#include <cctype>
#include <vector>
#include <sstream>
#include <cstdlib>
#include "rules.h"


int parse_board(const std::string b, int* newturn,
		char* newplayer, char newboard[RANKS][FILES]); 

std::vector<std::string>& split(const std::string& s, char delim,
		      std::vector<std::string>& elems);

std::vector<std::string> split(const std::string& s, char delim);
void print_string_vec(const std::vector<std::string>& v);
std::string board_to_string(const char board[RANKS][FILES]);
int find_piece(const char board[RANKS][FILES], char piece);

#endif

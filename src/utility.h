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
#include <boost/date_time/posix_time/posix_time.hpp>
#include <chrono>
#include "rules.h"


int parse_board(const std::string b, int* newturn,
		char* newplayer, char newboard[RANKS][FILES]); 
void print_string_vec(const std::vector<std::string>& v);
std::string board_to_string(const char board[RANKS][FILES]);
std::string flat_board_to_string(const char* board);
int find_piece(const char board[RANKS][FILES], char piece);
std::string current_time_string();
std::chrono::milliseconds ms_now();

void printCharVec(std::vector<char> v);

#endif

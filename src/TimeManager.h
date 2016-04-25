// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 23 Apr. 2016
// 
// dougchess/TimeManager.h
// 
// Time manager for dougchess. It will track remaining playtime,
// and decide how much time should be allocated to each move search.
//
// There should be only one global time manager.
//
// Time manager will examine the board state to determine how
// much time to allocate for each move. The Game is responsible
// for informing the time manager of changes in game state if necessary.
//
// Time management is accomplished by creating a thread which sleeps
// and then updates an atomic sentinel variable.

#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

class TimeManager;
class OutOfTimeException;

#include <string>
#include <stdio.h>
#include <cstring>
#include <sstream>
#include <vector>
#include <chrono>
#include <thread>
#include <atomic>
#include "rules.h"
#include "Board.h"
#include "utility.h"


class TimeManager {

 public:
  TimeManager();
  ~TimeManager();
  void start(const Board& board);
  bool out_of_time() const;
  void stop();

 private:
  std::chrono::milliseconds time_remaining;
  std::chrono::milliseconds start_time;
  std::chrono::milliseconds stop_time;
  bool timer_running;
  std::atomic<bool> out_of_time_atom; 
  void signal_time_out();
  std::thread::id current_timer_thread;
  int allot_time(const Board& board);
};



class OutOfTimeException : public std::runtime_error
{
 public:
 OutOfTimeException(const char* msg) : runtime_error(msg) {}
};

#endif



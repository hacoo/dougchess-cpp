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
  void start();
  bool out_of_time() const;

 private:
  int msecs_remaining;
  std::atomic<bool> out_of_time_atom; 
  void stop();
};


#endif



// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 23 Apr. 2016
// 
// dougchess/TimeManager.cpp
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


#include "TimeManager.h"

using namespace std;

TimeManager::TimeManager() : msecs_remaining(300000),
			     out_of_time_atom(false) { }

TimeManager::~TimeManager() { }

void TimeManager::start() {

  cout << "TimeManager START: " << current_time_string() << endl;
  
  out_of_time_atom = false;
  int t = 2000;
  msecs_remaining -= t;
  
  thread ([t, this]() {
      this_thread::sleep_for(chrono::milliseconds(t));
      this->stop();
    }).detach();
}

void TimeManager::stop() {
  out_of_time_atom = true;
  cout << "TimeManager STOP: " << current_time_string() << endl;
}

bool TimeManager::out_of_time() const {
  return out_of_time_atom.load();
}

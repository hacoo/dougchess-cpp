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

TimeManager::TimeManager() : time_remaining(300000),
			     out_of_time_atom(false) { }

TimeManager::~TimeManager() { }

void TimeManager::start(Board& board) {

  if (timer_running) {
    cout << "ERROR -- started TimeManager already running" << endl;
    exit(1);
  }
  
  start_time = ms_now();
  timer_running = true;
  out_of_time_atom = false;
  
  int t = allot_time(board);

  cout << "TimeManager START: " << current_time_string() << "\n"
       << "  Timer duration:  " << t << "\n"
       << "  Time remaining: " << time_remaining.count() << endl;
  
  thread timer([t, this]() {
      this_thread::sleep_for(chrono::milliseconds(t));
      this->signal_time_out();
    });
  current_timer_thread = timer.get_id();
  timer.detach();
}


// Examine the board and determine how many milliseconds to allot
// for this turn.
int TimeManager::allot_time(Board& board) {
  return 2000;
}

// Signals that time is out, should be called by the timer thread.
void TimeManager::signal_time_out() {
  if (current_timer_thread == this_thread::get_id()) {
    out_of_time_atom = true;
  }
  cout << "Timer expired: " << current_time_string() << endl;
}

// Called by TimeManager user (e.g. the board) to signal
// that it is no longer moving. Timemanager will assume
// some extra time has been used to account for delayes, etc.
void TimeManager::stop() {
  if (!timer_running) {
    cout << "ERROR -- stopped TimeManager not running" << endl;
    exit(1);
  }
  
  out_of_time_atom = true;
  timer_running = false;
  
  stop_time = ms_now();
  chrono::milliseconds time_used = stop_time - start_time;
  time_remaining -= time_used;
  // factor in unaccounted time
  time_remaining -= chrono::milliseconds(200);

  cout << "TimeManager STOP: " << current_time_string() << "\n"
       << "  Time consumed:  " << time_used.count() << "\n"
       << "  Time remaining: " << time_remaining.count() << endl;
}

bool TimeManager::out_of_time() const {
  return out_of_time_atom.load();
}

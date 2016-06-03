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
			     out_of_time_atom(false),
			     early_time_over_atom(false),
			     time_allotment(0){ }

TimeManager::~TimeManager() { }

void TimeManager::start(const Board& board) {

  if (timer_running) {
    cout << "ERROR -- started TimeManager already running" << endl;
    exit(1);
  }
  
  start_time = ms_now();
  timer_running = true;
  out_of_time_atom = false;
  early_time_over_atom = false;
  
  int t = allot_time(board);
  int early = t / 2;

  cout << "TimeManager START: " << current_time_string() << "\n"
       << "  Timer duration:  " << t << "\n"
       << "  Time remaining: " << time_remaining.count() << endl;
  
  thread timer([t, this]() {
      this_thread::sleep_for(chrono::milliseconds(t));
      this->signal_time_out();
    });
  current_timer_thread = timer.get_id();
  timer.detach();

  thread early_timer([early, this]() {
      this_thread::sleep_for(chrono::milliseconds(early));
      this->signal_early_time_out();
    });
  
  early_timer_thread = early_timer.get_id();
  early_timer.detach();
}


// Examine the board and determine how many milliseconds to allot
// for this turn.
//
// Will be much more generous duing the first 20 turns of the game.
int TimeManager::allot_time(const Board& board) {
  Board newboard(board);
  char player = newboard.getPlayer();
  int turn    = newboard.getTurn();
  int turns_remaining = max(40 - turn, 1);

  // More time is allocated to early turns
  // Since there's no opening book, we don't skimp in the beginning.
  float chunks[8] = {2.0, 1.75, 1.5, 1.25, 1.0, 0.75, 0.5, 0.25};
  int chunk = turn / 5;

  cout << "Allocating time in chunk: " << chunk << endl;
  float weight = chunks[chunk];
  
  // Minimum time allotted is 200 ms
  if(time_remaining.count() < 200)
    time_allotment = 200;
  else
    time_allotment = (int) (weight * time_remaining.count() / turns_remaining);
  cout << "Remaining: " << time_remaining.count() << endl;
  cout << "Allocated: " << time_allotment << endl;
  return time_allotment;
}

// Signals that time is out, should be called by the timer thread.
void TimeManager::signal_time_out() {
  if (current_timer_thread == this_thread::get_id()) {    
    out_of_time_atom = true;
  }
}

void TimeManager::signal_early_time_out() {
  if (early_timer_thread == this_thread::get_id()) {
    early_time_over_atom = true;
  }
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
  time_remaining -= chrono::milliseconds(50);

  cout << "TimeManager STOP: " << current_time_string() << "\n"
       << "  Time consumed:  " << time_used.count() << "\n"
       << "  Time remaining: " << time_remaining.count() << endl;
}

bool TimeManager::out_of_time() const {
  return out_of_time_atom.load();
}

void TimeManager::set_time_remaining(const int ms) {
  time_remaining = chrono::milliseconds(ms);
}

// Returns true if the 'early' timer, indicating a fraction of time
// has passed, is expired
bool TimeManager::early_out_of_time() const {
  return early_time_over_atom.load();
}

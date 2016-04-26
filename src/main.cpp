// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 2 Apr. 2016
// 
// dougchess/main.cpp
// 
// Main entry method for Dougchess. Begins communication
// with the Minichess framework.

#include <stdio.h>
#include <signal.h>
#include <thread>
#include <chrono>
#include "Client.h"
#include "TimeManager.h"
#include "ZobristTable.h"
#include "TranspositionTable.h"

using namespace std;

TimeManager manager;
ZobristTable zobrist;

Client client(manager);



void main_sigint(int signum) {
  printf("INTERRUPTED, SHUTTING DOWN\n");
  client.disconnect();
  printf("DISCONNECTED OK\n");
  exit(signum);
}

int main() {

  // Register interrupt handler, otherwise we could have 
  // unclosed ports
  signal(SIGINT, main_sigint);


  cout << "Creating transposition table" << endl;
  TranspositionTable tt(zobrist);
  cout << "Done!" << endl;
  
  
  printf("Connecting as Doug...\n");
  client.start(54361, "Doug");

  printf("Disconnecting...\n");
  client.disconnect();
  printf("Disconnected OK!\n");
  
  return 0;
}

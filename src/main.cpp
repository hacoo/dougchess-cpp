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
#include "testEndgame.h"

using namespace std;

TimeManager manager;
ZobristTable zobrist(1);
TranspositionTable tt;
Client client(manager, zobrist, tt);



void main_sigint(int signum) {
  printf("INTERRUPTED, SHUTTING DOWN\n");
  client.disconnect();
  printf("DISCONNECTED OK\n");
  exit(signum);
}

int main() {
  
  //testEndgame();
  //exit(0);

  // Register interrupt handler, otherwise we could have 
  // unclosed ports
  
  signal(SIGINT, main_sigint);
    
  printf("Connecting as Doug...\n");
  client.start(54361, "Doug");

  printf("Disconnecting...\n");
  client.disconnect();
  printf("Disconnected OK!\n");
  
  return 0;
}

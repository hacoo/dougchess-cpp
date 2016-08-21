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
#include <atomic>
#include <cstdlib>
#include <cctype>
#include <unistd.h>

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

// global flags to continue pondering
std::atomic<bool> currently_pondering_atom;
std::atomic<bool> continue_pondering_atom;

// Print usage message and exit
void usage() {
  printf("usage: dougchess [-p port] [-n name] \n");
  printf(" p port: port to connect to minichess framework (default 54361) \n");
  printf(" n name: name to register with framework (default Doug) \n");
  printf(" example: dougchess -p 12345 -n Example \n");
  exit(1);
}

void main_sigint(int signum) {
  printf("INTERRUPTED, SHUTTING DOWN\n");
  client.disconnect();
  printf("DISCONNECTED OK\n");
  exit(signum);
}

int main(int argc, char* argv[]) {

  //testEndgame();
  //exit(0);

  // Register interrupt handler, otherwise we could have 
  // unclosed ports
  signal(SIGINT, main_sigint);
  
  tt.clear();
  
  int port = 54361;
  char* name = NULL;
  int c;
  while ((c = getopt(argc, argv, "p:n:")) != -1) {
    switch(c) {
    case 'p':
      port = atoi(optarg);
      break;
    case 'n':
      name = optarg;
      break;
    case '?':
      usage();
    default:
      usage();
    }
  }

  if (name == NULL) { 
    printf("Connecting as Doug on port %d...\n", port);
    client.start(port, "Doug");
  } else {
    printf("Connecting as %s on port %d...\n", name, port);
    client.start(port, name);
  }

  printf("Disconnecting...\n");
  client.disconnect();
  printf("Disconnected OK!\n");
  
  return 0;
}


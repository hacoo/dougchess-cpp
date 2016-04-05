// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 2 Apr. 2016
// 
// dougchess/main.cpp
// 
// Main entry method for Dougchess. Begins communication
// with the Minichess framework.

#include <stdio.h>
#include <signal.h>
#include "Client.h"

using namespace std;

Client client;

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

  printf("Connecting as Doug...\n");
  client.start(54361, "Doug");

  printf("Disconnecting...\n");
  client.disconnect();
  printf("Disconnected OK!\n");
  
  return 0;
}

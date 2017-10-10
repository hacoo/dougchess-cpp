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
#include <cstdio>
#include <boost/program_options.hpp>
#include <stdexcept>

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

// Print usage message
void usage() {
  printf("usage: dougchess [-p port] [-n name] \n");
  printf(" p port: port to connect to minichess framework (default 54361) \n");
  printf(" n name: name to register with framework (default Doug) \n");
  printf(" example: dougchess -p 12345 -n Example \n");
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

  try {

    namespace bpo = boost::program_options;

    bpo::options_description desc{"Options"};
    desc.add_options()
      ("help,h", "Show help")
      ("port,p", bpo::value<int>()->default_value(54361), "Port on which to connect to framework")
      ("name,n", bpo::value<std::string>()->default_value("Doug"), "AI name");


    bpo::variables_map vm;
    bpo::store(bpo::parse_command_line(argc, argv, desc), vm);
    bpo::notify(vm);

    int port = 0;
    std::string name;

    if (vm.count("help")) {
      usage();
      return 1;
    }

    if (vm.count("port")) {
      port = vm["port"].as<int>();
    }

    if (vm.count("name")) {
      name = vm["name"].as<std::string>();
    }

    printf("Connecting as %s on port %d...\n", name.c_str(), port);
    client.start(port, name);

    printf("Disconnecting...\n");
    client.disconnect();
    printf("Disconnected OK!\n");

  } catch (const std::exception& ex) {
    std::cerr << ex.what() << std::endl;
    return 1;
  }

  return 0;
}


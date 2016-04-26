// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 2 Apr. 2016
// 
// dougchess/Client.cpp
// 
// Minichess client. Responsible for main communication loop
// with the Minichess framework.
#ifndef CLIENT_H

#include <zmq.h>
#include <string>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <numeric>
#include <vector>
#include "cjson.h"
#include "Game.h"
#include "TimeManager.h"
#include "ZobristTable.h"
#include "TranspositionTable.h"


class Client {
 public: 
  Client(TimeManager& manager,
	 ZobristTable& zobrist,
	 TranspositionTable& tt);
  ~Client();
  int start(int _port, std::string _name);
  int connect(int _port, std::string _name);
  int disconnect();
  Game game;
  
 private:
  void main_loop();
  void handle_json(cJSON* in, cJSON* out);
  bool running;
  bool connected;
  void* contextHandle;
  void* socketHandle;
  int port;
  std::string name;
};

#endif

// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 30 May 2016
// 
// dougchess/MongoEndgame.h
// 
// Interfaces with Mongodb endgame database. For now the interface
// is declared as a global in main, this class references it directly
// (I haven't figured out how to encapsulate it when using the new
// driver)
//
// As such, many methods on this class will be static, but the class
// may still carry some state for recording DB behavior, etc.

#ifndef MONGOENDGAME_H
#define MONGOENDGAME_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "rules.h"
#include "utility.h"
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

// Global mongoDB instance
extern mongocxx::instance inst;
extern mongocxx::client conn;
extern mongocxx::database db;


class MongoEndgame {
 public:
  MongoEndgame();
  ~MongoEndgame();
  
  static void printCollection(const std::string& coll);
  static void updatePosition(const std::string& piecelist,
			     const std::vector<EndgamePos>& positions,
			     const EndgameEntry& move);

 private:
};

#endif

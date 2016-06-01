// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 30 May 2016
// 
// dougchess/MongoEndgame.cpp
// 
// Interfaces with Mongodb endgame database.


#include "MongoEndgame.h"

using namespace std;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::finalize;




MongoEndgame::MongoEndgame() {
}


MongoEndgame::~MongoEndgame() {
}

// Print all database contents
void MongoEndgame::printCollection(const string& coll) {

  auto collection = db[coll];
  auto cursor = collection.find({});
  for (auto&& doc : cursor) {
    cout << bsoncxx::to_json(doc) << endl;
  }
}

void MongoEndgame::updatePosition(const string& piecelist,
				  const vector<EndgamePos>& positions,
				  const EndgameEntry& move) {
  for (auto pos : positions) {
    
    bsoncxx::builder::stream::document doc, filter;
    doc << "hello" << "henry!!";
    doc << "entry" << 2;
    filter << "hello" << "henry!!";
    auto res = db[piecelist].replace_one(filter.view(), doc.view());
    //auto res = db[piecelist].insert_one(doc.view());
    cout << pos.piece << endl;
  } 
}


// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 2 Apr. 2016
// 
// dougchess/Client.cpp
// 
// Minichess client. Responsible for main communication loop
// with the Minichess framework.

#include "Client.h"

using namespace std;

/* Default constructor */
Client::Client() {
  port = 0;
  name = "UNASSIGNED";
  contextHandle = NULL;
  socketHandle = NULL;
  running = false;
  connected = false;
}

/* Destructor */
Client::~Client() {
  disconnect();
}


// Connect on port p _port with name _name
// Return 0 on success, 1 on failure.
int Client::connect(int _port, std::string _name) {

  name = _name;
  port = _port;
  contextHandle = zmq_ctx_new();
  socketHandle = zmq_socket(contextHandle, ZMQ_PAIR);
  
  char charZMQ[1024] = { };
  sprintf(charZMQ, "tcp://*:%d", port);

  int zmq_error = zmq_bind(socketHandle, charZMQ);
  if (zmq_error == -1) {
    printf("zeromq error: %s\n", zmq_strerror(zmq_errno()));
    //disconnect();
    return 1;
  } 
  connected = true;
  return 0;
}

// Disconnect the client.
int Client::disconnect() {
  running = false;
  connected = false;
  if (connected) {
    zmq_close(socketHandle);
    zmq_ctx_destroy(contextHandle);
  }
  return 0;
}


// Start the client and begin the main loop.
// returns: 
// 0: success
// 1: connection error
int Client::start(int _port, string _name) {

  int err;
  if (connected) {
    printf("WARNING -- attemted to connect while already connected\n");
  }

  err = connect(_port, _name);

  if (err)
    return 1;
  
  main_loop();
  return 0;
}


// Start the main communication loop.
// Processes queries and responds to minichess framework.
void Client::main_loop() {

  if (running) {
      printf("WARNING -- attempted to enter main loop \
              when already running\n");
      return;
    }
  
  running = true;
  cJSON* jsonIn;
  cJSON* jsonOut;
  char charJson[1024] = { };
  char* charToSend = NULL;

  while (running) {
    jsonIn  = NULL;
    jsonOut = cJSON_CreateObject();

    if (zmq_recv(socketHandle, charJson, sizeof(charJson), 0) == -1) {
      printf("ERROR ON JSON RECEIVE \n");
      printf("zeromq: %s\n", zmq_strerror(zmq_errno()));
      return;
    }

    //printf("Received: %s\n", charJson);

    jsonIn = cJSON_Parse(charJson);

    handle_json(jsonIn, jsonOut);

    charToSend = cJSON_PrintUnformatted(jsonOut);

    //printf("Sending: %s\n", charToSend);
    
    if (zmq_send(socketHandle, charToSend, 
		 strlen(charToSend), 0) == -1) {
      printf("ERROR ON JSON SEND:\n");
      printf("zeromq: %s\n", zmq_strerror(zmq_errno()));
    }

    if(charToSend) {
      free(charToSend);
      charToSend = NULL;
    }

    if (jsonIn)
      cJSON_Delete(jsonIn);
    if (jsonOut)
      cJSON_Delete(jsonOut);
  }
  
  disconnect();
}


// Handle the currenct JSON message, in. 
// Record the response in out, if there is one.
void Client::handle_json(cJSON* in, cJSON* out) {

  // ping
  if (strcmp(cJSON_GetObjectItem(in, "strFunction")->valuestring,
	     "ping") == 0) {
    cJSON_AddStringToObject(out, "strOut", name.c_str());
  }
  
  // chess_reset
  else if (strcmp(cJSON_GetObjectItem(in, "strFunction")->valuestring,
	     "chess_reset") == 0) {
    game.reset();
  }
  
  // chess_boardGet
  else if (strcmp(cJSON_GetObjectItem(in, "strFunction")->valuestring,
		  "chess_boardGet") == 0) {
    cJSON_AddStringToObject(out, 
			    "strOut", 
			    game.mainBoardString().c_str());
  }

  // chess_boardSet
  else if (strcmp(cJSON_GetObjectItem(in, "strFunction")->valuestring,
		  "chess_boardSet") == 0) {
   game.boardSet(cJSON_GetObjectItem(in, "strIn")->valuestring);
  }
  
  // chess_winner
  else if (strcmp(cJSON_GetObjectItem(in, "strFunction")->valuestring,
		  "chess_winner") == 0) {
    char response[2];
    response[0] = game.winner();
    response[1] = '\0';
    cJSON_AddStringToObject(out, "strReturn", response);
  }

  // chess_isValid
  else if (strcmp(cJSON_GetObjectItem(in, "strFunction")->valuestring,
		  "chess_isValid") == 0) {
    cJSON_AddBoolToObject(out,
			  "boolReturn", 
			  isValid(cJSON_GetObjectItem(in, "intY")
				  ->valueint, 
				  cJSON_GetObjectItem(in, "intX")
				  ->valueint));    
  }

  // chess_isEnemy 
  else if (strcmp(cJSON_GetObjectItem(in, "strFunction")->valuestring,
		  "chess_isEnemy") == 0) {
    string piece(cJSON_GetObjectItem(in, "strPiece")->valuestring);
    cJSON_AddBoolToObject(out, "boolReturn", game.isEnemy(piece[0]));
  }

  // chess_isOwn
  else if (strcmp(cJSON_GetObjectItem(in, "strFunction")->valuestring,
		  "chess_isOwn") == 0) {
    string piece(cJSON_GetObjectItem(in, "strPiece")->valuestring);
    cJSON_AddBoolToObject(out, "boolReturn", game.isOwn(piece[0]));
  }

  // chess_isNothing
  else if (strcmp(cJSON_GetObjectItem(in, "strFunction")->valuestring,
		  "chess_isNothing") == 0) {
    string piece(cJSON_GetObjectItem(in, "strPiece")->valuestring);
    cJSON_AddBoolToObject(out, "boolReturn",game.isNothing(piece[0]));
  }

  // chess_eval
  else if (strcmp(cJSON_GetObjectItem(in, "strFunction")->valuestring,
		  "chess_eval") == 0) {
    cJSON_AddNumberToObject(out, "intReturn", game.eval());
  }

  //chess_moves
  else if (strcmp(cJSON_GetObjectItem(in, "strFunction")->valuestring,
		  "chess_moves") == 0) {
    vector<Move> ms = game.moves();
    vector<string> mstrs;
    for(vector<Move>::iterator i = ms.begin(); i != ms.end(); ++i) {
      mstrs.push_back(i->toString());
    }
    string moves = accumulate(mstrs.begin(), mstrs.end(), string(""));
    cJSON_AddNumberToObject(out, "intOut", mstrs.size());
    cJSON_AddStringToObject(out, "strOut", moves.c_str());
  }

  //chess_movesShuffled
  else if (strcmp(cJSON_GetObjectItem(in, "strFunction")->valuestring,
		  "chess_movesShuffled") == 0) {
    vector<Move> ms = game.movesShuffled();
    vector<string> mstrs;
    for(vector<Move>::iterator i = ms.begin(); i != ms.end(); ++i) {
      mstrs.push_back(i->toString());
    }
    string moves = accumulate(mstrs.begin(), mstrs.end(), string(""));
    cJSON_AddNumberToObject(out, "intOut", mstrs.size());
    cJSON_AddStringToObject(out, "strOut", moves.c_str());
  }

  //chess_movesEvaluated
  else if (strcmp(cJSON_GetObjectItem(in, "strFunction")->valuestring,
		  "chess_movesEvaluated") == 0) {
    vector<Move> ms = game.movesEvaluated();
    vector<string> mstrs;
    for(vector<Move>::iterator i = ms.begin(); i != ms.end(); ++i) {
      mstrs.push_back(i->toString());
    }
    string moves = accumulate(mstrs.begin(), mstrs.end(), string(""));
    cJSON_AddNumberToObject(out, "intOut", mstrs.size());
    cJSON_AddStringToObject(out, "strOut", moves.c_str());
  }

  //chess_move
  else if (strcmp(cJSON_GetObjectItem(in, "strFunction")->valuestring,
		  "chess_move") == 0) {
    string move(cJSON_GetObjectItem(in, "strIn")->valuestring);
    game.move(move);
  }
  
  //chess_undo
  else if (strcmp(cJSON_GetObjectItem(in, "strFunction")->valuestring,
		  "chess_undo") == 0) {
    game.undo();
  }

  //chess_moveRandom
  else if (strcmp(cJSON_GetObjectItem(in, "strFunction")->valuestring,
		  "chess_moveRandom") == 0) {
    cJSON_AddStringToObject(out, "strOut", game.moveRandom().c_str());
  }

  //chess_moveGreedy
  else if (strcmp(cJSON_GetObjectItem(in, "strFunction")->valuestring,
		  "chess_moveGreedy") == 0) {
    cJSON_AddStringToObject(out, "strOut", game.moveGreedy().c_str());
  }

  //chess_moveNegamax
  else if (strcmp(cJSON_GetObjectItem(in, "strFunction")->valuestring,
		  "chess_moveNegamax") == 0) {
    cJSON_AddStringToObject(out, "strOut", game.moveNegamax().c_str());
  }

  //chess_moveAlphabeta
  else if (strcmp(cJSON_GetObjectItem(in, "strFunction")->valuestring,
		  "chess_moveAlphabeta") == 0) {
    cJSON_AddStringToObject(out, "strOut", game.moveAlphabeta().c_str());
  }
}


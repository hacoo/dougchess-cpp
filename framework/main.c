#include "main.h"
#include "unistd.h"
#include "ctype.h"
#include "stdlib.h"
#include "stdio.h"
#include "getopt.h"

int main_intWebserver = 8080; // CHANGE THIS - OPTIONAL
int main_intZeromq = 54361; // CHANGE THIS - OPTIONAL

// Print usage message and exit
int usage() {
  printf("usage: framework [--ai_port a] [--webserver_port w] \n");
  printf(" ai_port a: optional port to connect to local AI (default 54361) \n");
  printf(" webserver_port w: optional port to connect client interface (default 8080) \n");
  printf(" example: --ai_port 1234 --webserver_port 5678 \n");
  exit(1);
  return 1;
}


int main(int argc, char** argv) {
  int c;
  while(1) {
    static struct option long_options[] =
      {
	{"webserver_port", required_argument, NULL, 'w'},
	{"ai_port",        required_argument, NULL, 'a'},
      };
    int option_index = 0;
    c = getopt_long(argc, argv, "w:a:", long_options, &option_index);
    if(c == -1)
      break;
    switch(c) {
    case 'w':
      main_intWebserver = atoi(optarg);
      break;
    case 'a':
      main_intZeromq = atoi(optarg);
      break;
    case '?':
      usage();
    default:
      usage();
    }
  }
  printf("Webserver port: %d \n", main_intWebserver);
  printf("AI port: %d \n", main_intZeromq);
  
  {
    assert(main_intWebserver > 1024);
    assert(main_intWebserver < 65535);
		
    assert(main_intZeromq > 1024);
    assert(main_intZeromq < 65535);
  }
	
  {
    srand(milliseconds());
  }
	
  {
    zeromq_start();
		
    webserver_start();
		
    imcs_start();
  }
	
  {
    printf("framework: started\n");
  }
	
  {
    pthread_exit(NULL);
  }
	
  {
    printf("framework: stopped\n");
  }
	
  return 0;
}

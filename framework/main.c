#include "main.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

int main_intWebserver = 8080; // CHANGE THIS - OPTIONAL
int main_intZeromq = 54361; // CHANGE THIS - OPTIONAL

void usage() {
	printf("Usage: framework [--cp=PORT --ap=PORT]\n");
	printf(" --cp: port on which web client will connect (default 8080) \n");
	printf(" --ap: port on which AI will connect (default 543761) \n");
	printf(" example: framework --cp=8081 --ap=1234 \n");
}

int main(int argc, char** argv) {
	{
		int c;

		while (1)
			{

			static struct option long_options[] =
				{
					{"ap", optional_argument, NULL, 'a'},
					{"cp", optional_argument, NULL, 'c'}
				};

			int option_index = 0;
			c = getopt_long(argc, argv, "a:c:", long_options, &option_index);

			if (c == -1)
				break;

			switch (c)
				{
				case 'c':
					main_intWebserver = atoi(optarg);
					break;
				case 'a':
					main_intZeromq = atoi(optarg);
					break;
				case '?':
					printf("Unknown command line character\n");
					return 1;
				default:
					printf("ABORT!");
					abort ();
				}
			}

		printf("Frameword starting: \n");
		printf(" Client port: %d\n", main_intWebserver);
		printf(" AI port: %d\n", main_intZeromq);

		if (main_intWebserver < 1025  ||
			main_intWebserver > 65534 ||
			main_intZeromq < 1025     ||
			main_intZeromq > 65534)
			{
				printf("\nInvalid port! Ports must be between 1025 and 65534\n\n");
				usage();
				return 1;
			}
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

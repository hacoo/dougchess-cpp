## Dougchess

## Overview
Dougchess is a high performance C++ Minichess AI. It took 1st place in the 2016 PSU Minichess Tournament,
where it was undefeated in matches against over 40 other AIs.

Dougchess is named after Doug the dog, who is very good at being a dog, but needs help with chess.

The guiding principles in development of Dougchess were high performance and iterative testing.
During development, careful attention was taken to keep code fast, simple, and modular.
Each potential new feature was tested to determine whether it was worth the performance cost, and I ruthlessly cut those that were not.
Examples of cut features include the endgame database (a large transposition table makes this unnecessary, due
to the reduced search space in Minichess) and numerous evaluation heuristics (since eval() is called
exponentially many times, it is vital that it do as little work as possible).

## License
This software is distributed under the GNU General Public License. See the file LICENSE in this repository for more information. 

## Prerequisites
Dougchess has been tested on Ubuntu Linux ONLY.

Douchess requires libzmq version 3.x.x. In addition to Dougchess, you must
install the Minichess framework, which provides a frontend for interacting
with the AI.

To install libzmq version 3:

> sudo apt-get install libzmq3-dev

Additionally, Dougchess makes use two Boost libraries, chrono and program_options. to install them: 

> sudo apt-get install libboost-chrono-dev libboost-program-options-dev

The Minichess framework is included in the framework/ directory. You can find the original repo here: https://github.com/CodeRect/teaching-minichess

## Building Dougchess
To build, use the included makefile:

> make

The binary will appear at bin/dougchess.

## Building the Framework
To build the framework, cd into the framework/ directory and use the makefile:

> make

The framework binary will appear at framework/framework.

## Starting the AI
To run Dougchess, you must run both Dougchess itself and the Minichess framework.

To run Dougchess: 
> bin/dougchess -p PORT -n NAME

PORT is the port which will be used for communication with the Minichess framework, while NAME is the client name to register with the framework.

## Starting the Framework
After building the framework, start it as follows:

> framework/framework --ap=PORT --cp=PORT

The --ap argument is the port on which the AI will connect. The --cp argument is the port on which the web client will connect. The default AI port is 54361, and the default web client port is 8080.

## Starting the Web Client
The minichess framework is controlled via a web interface. To use it, you must first start the client (see above).Then, open your web browser and type the following into the address bar:

> localhost:PORT

Where PORT is the client port you specified when starting the framework (the --cp option).

See https://github.com/CodeRect/teaching-minichess for more information on using the web client.

## Playing against the AI
Clicking on pieces in the browser interface will allow you to move them. You may ask the AI
to make a move at any time by hitting the chess_moveAlphabeta button. This button includes two
parameters: the maximum search duration and the maximum search depth. Increase the search depth and duration
for more challenging play but slower moves.

## Challenging other AIs
To initiate a match against another AI, you must use the 'Internet' box at the bottom of the Framework
interface. See the Framework readme for more details on using this feature.

## Playing the AI against itself
Since you probably won't find many other AIs online, the easiest way to see Dougchess
in action is to start two separate instances of the framework, log in via two accounts, and initiate a match.
If you have access to two separate machines, this is easy -- just install Dougchess and the framework on
both, create two IMCS accounts, and start a match between them.

Starting a match on the local machine is slightly trickier, since you must start two AIs and frameworks
with separate ports. The ai_port and client_port for each AI must not conflict. So, for example, running
two AIs might look something like this:

Starting the first AI:
> douchess/bin/dougchess -port 10000 -name aione

> framework/framework --ap=10000 --cp=8080

Then, in a second terminal, start the other AI: 
> dougchess/bin/dougchess -port 10001 -name aitwo

> framework/framework -ap=10001 -cp=8081

Then, start two browser windows, one connecting to localhost:8080 and the other to localhost:8081.
Log in to separate IMCS accounts on each, and watch your AIs duke it out.

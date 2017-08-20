## Dougchess

## Overview
Dougchess is a high performance C++ Minichess AI. It took 1st place in the 2016 PSU Minichess Tournament,
where it was undefeated in matches against over 40 other AIs.

The guiding principles in development of Dougchess were high performance and iterative testing.
During development, careful attention was taken to keep code fast, simple, and modular.
Each potential new feature was tested to determine whether it was worth the performance cost, and I ruthlessly cut those that were not.
Examples of cut features include the endgame database (a large transposition table makes this unnecessary, due
to the reduced search space in Minichess) and numerous evaluation heuristics (since eval() is called
exponentially many times, it is vital that it do as little work as possible).

## License
This software is distributed under the GNU General Public License. See the file COPYING in this repository for more information. 

## Prerequisites
Dougchess has been tested on Ubuntu Linux ONLY.

Douchess requires libzmq version 3.x.x. In addition to Dougchess, you must
install the Minichess framework, which provides a frontend for interacting
with the AI.

Additionally, Dougchess makes use of Boost Chrono for time managment. To install, on Ubuntu:

> sudo apt-get install libboost-chrono-dev

To download and install libzmq 3, look here: http://zeromq.org/intro:get-the-software

The minichess AI framework can be found here: https://github.com/CodeRect/teaching-minichess

In addition, the Framework source code is included in this repository, in the framework directory.
The minichess framework is distributed under the GNU GPL v. 3.0.

## Building 
To build, use the included makefile:

> make

The binary will appear at bin/dougchess.

## Starting the AI
To run Dougchess, you must run both Dougchess itself and the Minichess framework.

To run Dougchess: 
> bin/dougchess -p PORT -n NAME

PORT is the port which will be used for communication with
the Minichess framework, while NAME is the client name to register
with the framework.

To run the framework, simply build it and run the generated executable.
Then, navigate to http://localhost:8080 in a web browser. Once the AI connects,
you should see the Minichess interface. See the Minichess Framework readme for more
details on building the framework.

## Playing against the AI
Clicking on pieces in the browser interface will allow you to move them. You may ask the AI
to make a move at any time by hitting the chess_moveAlphabeta button. This button includes two
parameters: the maximum search duration and the maximum search depth. Increase the search depth
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

> framework/framework -ai_port 10000 -client_port 8080

Then, in a second terminal, start the other AI: 
> dougchess/bin/dougchess -port 10001 -name aitwo

> framework/framework -ai_port 10001 -client_port 8081

Then, start two browser windows, one connecting to localhost:8080 and the other to localhost:8081.
Log in to separate IMCS accounts on each, and watch your AIs duke it out.

In the future, I plan to add a 'self-play' button to make this easier :)

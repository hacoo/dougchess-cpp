// Henry Cooney <email: hacoo36@gmail.com> <Github: hacoo>
// 28 May 2016
// 
// dougchess/posgen.h
// 
// Functions for generating positions. Operates on simple char
// arrays (not full board objects)

#ifndef POSGEN_H
#define POSGEN_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include "rules.h"
#include "utility.h"


std::vector<lwBoard*>
generateAllPositions(std::vector<char>& pieces);
void generateAllPositions(lwBoard* board,
			  std::vector<char>& pieces,
			  std::vector<lwBoard*>& positions);


#endif

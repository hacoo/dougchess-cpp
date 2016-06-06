# Henry Cooney <hacoo36@gmail.com> <Github: hacoo>
# 1 Apr. 2016
#
# Top-level makefile for Dougchess.

CC        := g++
C_CC      := gcc
SRCDIR    := src
BUILDDIR  := build
TARGET    := bin/dougchess

SRCEXT    := cpp
C_SRCEXT  := c
SOURCES   := $(wildcard $(SRCDIR)/*.$(SRCEXT))
C_SOURCES := $(wildard $(SRCDIR/*.$(C_SRCEXT)))
OBJECTS   := $(patsubst $(SRCDIR)/%, $(BUILDDIR)/%, $(SOURCES:.$(SRCEXT)=.o))
OBJECTS   += $(patsubst $(SRCDIR)/%, $(BUILDDIR)/%, $(C_SOURCES:.$(C_SRCEXT)=.o))
CFLAGS    := -g -Wall -std=c++1y -O3
LIB       := -pthread -lm -lzmq -lmongocxx -lbsoncxx -lprofiler -L lib 
INC       := -I include -I /usr/local/lib/include/bsoncxx/v_noabi/ -I /usr/local/lib/include/mongocxx/v_noabi/

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	$(CC) $^ -o $(TARGET) $(LIB)
	@echo "---- COMPILED OK ----"

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning..."
	$(RM) -r $(OBJECTS) $(TARGET)

run: $(TARGET)
	@bin/dougchess

# Tests
tester:
	$(CC) $(CFLAGS) test/tester.cpp $(INC) $(LIB) -o bin/tester

callgrind:
	@valgrind --tool=callgrind --instr-atstart=no bin/dougchess

.PHONY: clean




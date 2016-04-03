# Henry Cooney <hacoo36@gmail.com> <Github: hacoo>
# 1 Apr. 2016
#
# Top-level makefile for Dougchess.

CC       := g++
SRCDIR   := src
BUILDDIR := build
TARGET   := bin/dougchess

SRCEXT   := cpp
SOURCES  := $(wildcard $(SRCDIR)/*.$(SRCEXT))
OBJECTS  := $(patsubst $(SRCDIR)/%, $(BUILDDIR)/%, $(SOURCES:.$(SRCEXT)=.o))
CFLAGS   := -g -Wall # -O3
LIB      := -pthread -L lib
INC      := -I include

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	$(CC) $^ -o $(TARGET) $(LIB)
	@echo "---- COMPILED OK ----"

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@echo " Compiling..."
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

.PHONY: clean




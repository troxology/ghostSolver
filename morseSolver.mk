CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=src/morseSolver.cpp src/CommonUtils.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=bin/morseSolver

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) -std=c++11 -Iinclude/ $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) -std=c++11 -Iinclude/ $(CFLAGS) $< -o $@
	
clean:
	@echo " Cleaning..."; 
	@echo " $(RM) $(EXECUTABLE)"; $(RM) $(EXECUTABLE)
CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=src/caesarSolver.cpp src/CommonUtils.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=bin/caesarSolver

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) -std=c++11 -Iinclude/ $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) -std=c++11 -Iinclude/ $(CFLAGS) $< -o $@
	
clean:
	@echo " Cleaning..."; 
	@echo " $(RM) $(EXECUTABLE)"; $(RM) $(EXECUTABLE)
# Sub-directories and source files.
DIRS = search
SRC = $(shell find *.cpp)
# Makefile output
OBJ = $(SRC:.cpp=.o)

# C++ compiler
CXX = g++
# C++ compiler flags
ifeq ($(DEBUG), 0)
CXXFLAGS = -O2 -Wall -fPIC -std=c++0x
else
CXXFLAGS = -g -O0 -Wall -fPIC -std=c++0x
endif
# Include directories
INCLUDES = -I/usr/local/include -I./..

all: subs $(OBJ)


subs:
	for dir in $(DIRS); do\
        make -C $$dir;\
    done


.cpp.o:
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $< -o $@


clean:
	rm -f *.o
	for dir in $(DIRS); do\
        make clean -C $$dir;\
    done


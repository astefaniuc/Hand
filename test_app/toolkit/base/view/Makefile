# Sub-directories and source files.
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
INCLUDES = -I. -I./../../include/ -I./../../themes/lib/ -I/usr/include/SDL


default: $(OBJ)


.cpp.o:
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $< -o $@


clean:
	rm -f *.o

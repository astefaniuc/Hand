# source files.
SRC = $(shell find *.cpp)
OBJ = $(SRC:.cpp=.o)
# Makefile output
OUT = libthemes.a

# C++ compiler
CXX = g++
# C++ compiler flags
ifeq ($(DEBUG), 0)
CXXFLAGS = -O2 -Wall -fPIC -std=c++0x
else
CXXFLAGS = -g -O0 -Wall -fPIC -std=c++0x
endif
# Include directories
INCLUDES = -I. -I./../../include/  -I./../../base/layer/ -I/usr/include/SDL


default: $(OUT)

.cpp.o:
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	ar rcs $(OUT) $(OBJ)


clean:
	rm -f *.o
	rm -f *.a

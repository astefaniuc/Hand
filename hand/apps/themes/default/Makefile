# Sub-directories and source files.
SRC = $(shell find *.cpp)
OBJ = $(SRC:.cpp=.o)
# Makefile output
OUT = default.so

# C++ compiler
CXX = g++
# C++ compiler flags
ifeq ($(DEBUG), 0)
CXXFLAGS = -O2 -Wall -fPIC -std=c++0x
else
CXXFLAGS = -g -O0 -Wall -fPIC -std=c++0x
endif
# Include directories
INCLUDES = -I./../../../toolkit 
# Library paths
LIBS= -L./../../../toolkit
# Linker flags
LDFLAGS = -g -shared -Wl,-soname,$(OUT) -Wl,-E
# -Wl,--export-dynamic 


all: $(OUT)


.cpp.o:
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $< -o $@

$(OUT):	$(OBJ) 
	$(CXX) $(LDFLAGS) -o $(OUT) $(OBJ) $(LIBS)
	cp $(OUT) ./../$(OUT)


clean:
	rm -f *.o
	rm -f *.so

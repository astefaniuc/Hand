# Sub-directories and source files.
DIRS = input layer search view
SRC = $(shell find *.cpp)
OBJ = $(SRC:.cpp=.o)
OBJ_A = $(shell find . -name "*.o")
# Makefile output
OUT = libHand.so

# C++ compiler
CXX = g++
# C++ compiler flags
ifeq ($(DEBUG), 0)
CXXFLAGS = -O2 -Wall -fPIC -std=c++0x
else
CXXFLAGS = -g -O0 -Wall -fPIC -std=c++0x
endif
# Include directories
INCLUDES = -I. -I./../include/ -I./layer/ -I/usr/include/SDL -I/usr/local/include
# Library paths
LIBS=`sdl-config --cflags --libs` -ldl -lboost_filesystem -lboost_system
# Linker flags
LDFLAGS = -g -shared -Wl,-soname,$(OUT) -Wl,-E
# -Wl,--export-dynamic 

all: subs default


subs:
	for dir in $(DIRS); do\
        make -C $$dir;\
    done

default: $(OUT)

.cpp.o:
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	$(CXX) $(LDFLAGS) -o $(OUT) $(OBJ_A) $(LIBS)
	cp $(OUT) ./../$(OUT)


clean:
	rm -f *.o
	rm -f *.so
	for dir in $(DIRS); do\
        make clean -C $$dir;\
    done

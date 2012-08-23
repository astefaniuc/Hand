ifndef DEBUG
DEBUG = 1
endif
export DEBUG

# Sub-directories and source files.
DIRS = toolkit apps
# Makefile output
OUT = hand

# C++ compiler
CXX = g++
# C++ compiler flags
ifeq ($(DEBUG), 0)
CXXFLAGS = -O2 -Wall -std=c++0x
else
CXXFLAGS = -g -O0 -Wall -std=c++0x
endif
# Include directories
INCLUDES = -I${PWD}/toolkit -L${PWD}/toolkit
# Library paths
LIBS = -lHand -lboost_system


all: shared_libs starter


shared_libs:
	for dir in $(DIRS); do\
        make -C $$dir;\
    done

starter: $(OUT).cpp
	$(CXX) $^ $(INCLUDES) $(LIBS) $(CXXFLAGS) -o $(OUT) -Wl,-E


clean:
	rm -f $(OUT)
	rm -f *.so
	for dir in $(DIRS); do\
        make clean -C $$dir;\
    done

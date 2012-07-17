# C++ compiler
CXX = g++
# C++ compiler flags
ifeq ($(DEBUG), 0)
CXXFLAGS = -O2 -Wall -fPIC -std=c++0x
else
CXXFLAGS = -g -O0 -Wall -fPIC -std=c++0x
endif
# Include directories
INCLUDES = -I./../../toolkit/include 
# Library paths
LIBS = -L./../../toolkit
# Linker flags
LDFLAGS = -shared -Wl,-soname,$@


all: test1_lib


test1_lib: test1.so

test1.so: test1.o
	$(CXX) $(LIBS) $(LDFLAGS) -o $@ test1.o

test1.o: test1.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c test1.cpp -o $@


clean:
	rm -f *.so
	rm -f *.o


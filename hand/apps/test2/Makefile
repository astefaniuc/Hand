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


all: test2_lib


test2_lib: test2.so

test2.so: test2.o
	$(CXX) $(LIBS) -$(LDFLAGS) -o $@ test2.o

test2.o: test2.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c test2.cpp -o $@


clean:
	rm -f *.so
	rm -f *.o

# Sub-directories and source files.
SRC = $(shell find *.cpp)
DIR = lib
# Themes should load dynamically => using OPP as file extension for objects
# (all .o file, including subfolders added to libHand.a)
OBJ = $(SRC:.cpp=.opp)
# Makefile output
OUT = $(OBJ:.opp=.so)

# C++ compiler
CXX = g++
# C++ compiler flags
ifeq ($(DEBUG), 0)
CXXFLAGS = -O2 -Wall -fPIC -std=c++0x
else
CXXFLAGS = -g -O0 -Wall -fPIC -std=c++0x
endif
# Include directories
INCLUDES = -I. -I./../include/ -I./../base/layer/ -I./lib/ -I/usr/include/SDL
# Linker flags
LDFLAGS = -shared -Wl,-E -Wl,-soname,$@ -o $@
# Library paths
LIBS = ./lib/libthemes.a `sdl-config --cflags --libs` -lSDL_ttf  -L./lib/


all: themes_lib themes


themes_lib:
	make -C $(DIR);

themes: $(OUT)

$(OUT): $(OBJ)
	$(CXX) $(LDFLAGS) $(subst so,opp,$@) $(LIBS)

$(OBJ): $(SRC)
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $(subst opp,cpp,$@) -o $@


clean:
	rm -f *.opp
	rm -f *.so
	make clean -C $(DIR);
	
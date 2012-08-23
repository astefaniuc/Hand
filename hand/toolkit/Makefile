# Sub-directories and source files.
DIRS = base graph input view
OBJ_A = $(shell find . -name "*.o")
# Makefile output
OUT = libHand.so

# C++ compiler
CXX = g++
# Library paths
LIBS=`sdl-config --cflags --libs` -ldl -lboost_filesystem -lboost_system -lSDL_ttf
# Linker flags
LDFLAGS = -g -shared -Wl,-soname,$(OUT) -Wl,-E
# -Wl,--export-dynamic 

all: subs $(OUT)


subs:
	for dir in $(DIRS); do\
        make -C $$dir;\
    done


$(OUT): $(OBJ_A)
	$(CXX) $(LDFLAGS) -o $(OUT) $(OBJ_A) $(LIBS)
	cp $(OUT) ./../$(OUT)


clean:
	rm -f *.o
	rm -f *.so
	for dir in $(DIRS); do\
        make clean -C $$dir;\
    done

DIRS = base themes


all: default


default:
	for dir in $(DIRS); do\
        make -C $$dir;\
    done


clean:
	rm -f *.so
	for dir in $(DIRS); do\
        make clean -C $$dir;\
    done

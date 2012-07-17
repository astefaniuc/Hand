DIRS = test1 test2


all:
	for dir in $(DIRS); do\
        make -C $$dir;\
    done


clean:
	for dir in $(DIRS); do\
        make clean -C $$dir;\
    done


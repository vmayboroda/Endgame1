all: install clean

install:
	cp src/*.c .
	cp inc/*.h .
	clang -std=c11 -Wall -Wextra -Werror -Wpedantic -lncurses -o endgame *.c

uninstall: clean
	rm -f endgame
	
clean:
	rm -f *.h
	rm -f *.c
	
reinstall:
	make uninstall
	make

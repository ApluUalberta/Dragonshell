compile: dragonshell.cc execute
	g++ -Wall -std=c++11 dragonshell.o -o dragonshell


execute: dragonshell.cc
	g++ -c -Wall -std=c++11 dragonshell.cc

clean:
	rm -f dragonshell dragonshell.o

taring: dragonshell.cc readme.md

	tar -cvf APLU-dragonshell.tar.gz dragonshell.cc readme.md Makefile
.PHONY: clean

all:
	gcc -W -Wall -o hugepages hugepages.c
clean:
	rm -rf issues


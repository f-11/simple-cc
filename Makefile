simple-cc: simple-cc.c

test: simple-cc
	./test.sh

clean:
	rm -f simple-cc *.o 

.PHONY: test clean

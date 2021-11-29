## makefile for hash table assignment (Celina Wong ECE 361 HW #4)

C = gcc
CFLAGS = -c -Wall -std=c99 -g
OBJS = test_hashtable.o appHelpers.o hash_table.o
HDRS = hash_table.h appHelpers.h

#test object file
test_hashtable.o: test_hashtable.c
	$(C) $(CFLAGS) test_hashtable.c  	#gcc command line

#hash_table object file with its .c and .h files
hash_table.o: hash_table.c hash_table.h
	$(C) $(CFLAGS) hash_table.c   #gcc command line

#appHelpers object file with its .c and .h files
appHelpers.o: appHelpers.c appHelpers.h
	$(C) $(CFLAGS) appHelpers.c   #gcc command line

test_hashtable: $(OBJS) $(HDRS)
	$(C) $(OBJS) $(HDRS) -o test_hashtable

exec:
	./test_hashtable

clean:	#clean target, deletes all but .c and .h files
	rm -rf *.o *.exe *.gch

#to compile makefile on my computer, type in "mingw32-make" into command
#then ./test_hashtable to execute

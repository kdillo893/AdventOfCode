
CC = clang
CFLAGS = -Wall -Werror -pedantic
OPTIM = -O2
DBUG = -g

clean: 
	rm parsecode

parsecode:
	$(CC) $(CFLAGS) $(OPTIM) parsecode.c -o parsecode 

debug:
	$(CC) $(CFLAGS) $(OPTIM) $(DBUG) parsecode.c -o parsecode


CC = clang
CFLAGS = -Wall -Werror -pedantic
OPTIM = -O2
DBUG = -g

clean: 
	rm cube

cube:
	$(CC) $(CFLAGS) $(OPTIM) cube.c -o cube 

debug:
	$(CC) $(CFLAGS) $(OPTIM) $(DBUG) cube.c -o cube


CC = clang
CFLAGS = -Wall -Werror -pedantic
OPTIM = -O2
DBUG = -g

clean: 
	rm gear

gear:
	$(CC) $(CFLAGS) $(OPTIM) gear.c -o gear 

debug:
	$(CC) $(CFLAGS) $(OPTIM) $(DBUG) gear.c -o gear

all: gear

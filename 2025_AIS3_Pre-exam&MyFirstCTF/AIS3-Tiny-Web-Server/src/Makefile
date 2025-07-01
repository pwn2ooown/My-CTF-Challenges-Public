CC ?= cc
CFLAGS ?= -Wall -O2
#-DNO_LOG_ACCESS
#-DDEFAULT_PORT=9000
#-DFORK_COUNT=2

all: tiny

tiny: tiny.c
	$(CC) $(CFLAGS) -o tiny tiny.c
ais3: tiny.c
	$(CC) -fno-stack-protector -m32 $(CFLAGS) -o tiny tiny.c
clean:
	rm -f *.o tiny *~

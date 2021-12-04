CC = gcc
SRCMODULES = states.c prep.c setsock.c send.c recv.c logger.c
OBJMODULES = $(SRCMODULES:.c=.o)
CFLAGS = -Wall -g -lm

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

ping: ping.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@

ifneq (clean, $(MAKECMDGOALS))
-include deps.mk
endif

deps.mk: $(SRCMODULES)
	$(CC) -MM $^ > $@

clean: rm -f *.o ping

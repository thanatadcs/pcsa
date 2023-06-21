CC=gcc
CFLAGS=-std=gnu17
TARGETS=micro_cc

all: $(TARGETS)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

micro_cc: micro_cc.o pcsa_net.o
	$(CC) -o $@ $^

clean:
	rm -f $(TARGETS) *.o

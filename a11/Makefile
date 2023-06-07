CC=gcc
CFLAGS=-std=gnu17
TARGETS=micro

all: $(TARGETS)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

micro: micro.o pcsa_net.o
	$(CC) -o $@ $^

clean:
	rm -f $(TARGETS) *.o

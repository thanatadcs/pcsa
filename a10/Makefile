CC=gcc
CFLAGS=-std=gnu17
TARGETS=send recv # send_hello

all: $(TARGETS)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

recv: recv.o pcsa_net.o
	$(CC) -o $@ $^

send: send.o pcsa_net.o
	$(CC) -o $@ $^

clean:
	rm -f $(TARGETS) *.o

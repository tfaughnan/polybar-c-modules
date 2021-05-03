CC = gcc
CFLAGS = -g -Wall -std=gnu99
LIBS = -lpulse
TARGETS = bin/time bin/volume

all: $(TARGETS)

bin/battery: obj/battery.o
	$(CC) $(CFLAGS) -o $@ $<

bin/time: obj/time.o
	$(CC) $(CFLAGS) -o $@ $<

bin/volume: obj/volume.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGETS)

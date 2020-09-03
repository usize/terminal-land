src = $(wildcard src/*.c)
obj = $(src:.c=.o)

binname = terminal-land

OPTIONS=-Wall
LDFLAGS = -lm -lncurses 

$(binname): $(obj)
	$(CC) $(OPTIONS) -o $@ $^ $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(obj) $(binname)

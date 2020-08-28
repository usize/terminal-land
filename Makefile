src = $(wildcard src/*.c)
obj = $(src:.c=.o)

binname = client

LDFLAGS = -lm -lncurses 

$(binname): $(obj)
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(obj) $(binname)

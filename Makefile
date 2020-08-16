src = $(wildcard src/*.c src/framebuffer/*.c)
obj = $(src:.c=.o)

binname = graphics

LDFLAGS = -lm 

$(binname): $(obj)
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: clean
	rm -f $(obj) $(binname)

src = $(wildcard src/libs/*.c)
obj = $(src:.c=.o)

gamesrc = src/terminal_land.c
gameobj = src/terminal_land.o
gamebin = terminal-land

editorsrc = src/editor.c
editorobj = src/editor.o
editorbin = editor 

OPTIONS=-Wall
LDFLAGS = -lm -lncurses 

.PHONY: default
default: $(gamebin) $(editorbin)

$(gameobj):
	$(CC) $(OPTIONS) -c $(gamesrc)

$(editorobj):
	$(CC) $(OPTIONS) -c $(editorsrc)

$(gamebin): $(gameobj) $(obj)
	mv *.o src/
	$(CC) $(OPTIONS) -o $@ $^ $(LDFLAGS)

$(editorbin): $(editorobj) $(obj)
	mv *.o src/
	$(CC) $(OPTIONS) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) $(gameobj) $(editorobj) $(binname)

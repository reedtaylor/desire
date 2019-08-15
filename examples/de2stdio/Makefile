
CC := g++
CFLAGS := 
LIBS := -levent -lglog -lgflags

PROGS := de2stdio

all : $(PROGS)

% : %.cpp
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

clean :
	rm $(PROGS)

.PHONY : all clean


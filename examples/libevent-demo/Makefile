
CC := g++
CFLAGS := 
LIBS := -levent -lglog

PROGS := libevent-demo

all : $(PROGS)

% : %.cpp
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

clean :
	rm $(PROGS)

.PHONY : all clean


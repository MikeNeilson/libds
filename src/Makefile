SRC = $(wildcard *.c)
OBJ = $(src:.c=.o)

all: libds.so

libds.so: $(OBJ)
	$(CC) -fPIC -shared -o $@ $<

.o:.c
	$(CC) -ansi -fPIC -c $<

test: $(OBJ)
	$(CC) -o $@ $<
	


clean: 
	rm $(OBJ) libds.so


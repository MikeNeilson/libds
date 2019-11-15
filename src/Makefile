SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

CFLAGS = --std=c99 -Wall
all: libds.so

libds.so: CFLAGS += -DTINYTEST_NO_TEST
libds.so: $(OBJ)
	$(CC) -fPIC -shared -o $@ $^

.c.o:
	$(CC) -fPIC -c $< -o $@ $(CFLAGS)

.PHONY: test
test: CFLAGS = -g -fno-builtin-strlen --std=c99
test: $(OBJ)
	$(CC) -o $@ $^ && valgrind --leak-check=full ./test

	

.PHONY: clean

clean: 
	rm $(OBJ) libds.so test



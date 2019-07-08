SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)


all: libds.so

libds.so: CFLAGS += -DTINYTEST_NO_TEST
libds.so: $(OBJ)
	$(CC) -fPIC -shared -o $@ $^

.c.o:
	$(CC) -ansi -fPIC -c $< -o $@ $(CFLAGS)

.PHONY: test
test: CFLAGS = -g -fno-builtin-strlen
test: $(OBJ)
	$(CC) -o $@ $^ && valgrind --leak-check=full ./test

	

.PHONY: clean

clean: 
	rm $(OBJ) libds.so test



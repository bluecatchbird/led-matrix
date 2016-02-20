CFLAGS= -Wall -Wextra


all: random-matrix virtual-matrix

random-matrix: random-matrix.c
	gcc $^ -o $@ $(CFLAGS)

virtual-matrix: virtual-matrix.c
	gcc $^ -o $@ -lX11 $(CFLAGS)

test: random-matrix virtual-matrix
	./random-matrix 20 30 | ./virtual-matrix 20 30 20

clean:
	rm -f random-matrix virtual-matrix

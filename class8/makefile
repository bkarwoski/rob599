CFLAGS = -ggdb3 -std=c11 -Wall -Wunused-parameter -Wstrict-prototypes -Werror -Wextra -Wshadow
CFLAGS += -fsanitize=signed-integer-overflow
CFLAGS += -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable
CFLAGS += -fsanitize=address -fsanitize=undefined

bisection: bisection.c
	gcc -o $@ $^ $(CFLAGS) -std=c11 -lm

binarysearch: binarysearch.c
	gcc -o $@ $^ $(CFLAGS) -std=c11 -lm

fibonacci: fibonacci.c
	gcc -o $@ $^ $(CFLAGS) -std=c11 -lm
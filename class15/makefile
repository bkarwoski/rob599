CFLAGS = -ggdb3 -std=c11 -Wall -Wunused-parameter -Wstrict-prototypes -Werror -Wextra -Wshadow
#CFLAGS += -fsanitize=signed-integer-overflow
CFLAGS += -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wfloat-conversion
#CFLAGS += -fsanitize=address -fsanitize=undefined

buffer: buffer.c
	gcc -o $@ $^ $(CFLAGS) -std=c11 -lm -lpthread -O3
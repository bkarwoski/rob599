CFLAGS = -ggdb3 -std=c11 -Wall -Wunused-parameter -Wstrict-prototypes -Werror -Wextra -Wshadow
CFLAGS += -fsanitize=signed-integer-overflow
CFLAGS += -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable
CFLAGS += -fsanitize=address -fsanitize=undefined

bmp: bmp.c
	gcc -o $@ $^ $(CFLAGS) -std=c11 -lm

rasterize: rasterize.c bmp.c image_server.c
	gcc -o $@ $^ $(CFLAGS) -std=c11 -lm -lpthread
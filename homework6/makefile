CFLAGS = -ggdb3 -std=c11 -Wall -Wunused-parameter -Wstrict-prototypes -Werror -Wextra -Wshadow
CFLAGS += -fsanitize=signed-integer-overflow
CFLAGS += -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable
#CFLAGS += -fsanitize=address -fsanitize=undefined

# The '%' is a wild card. To make any lcmtypes .c file, use the corresponding .lcm file
# For organization, we reference and use all these files under a lcmtypes directory
lcmtypes/%_t.c: lcmtypes/%_t.lcm
	lcm-gen -c --c-cpath lcmtypes/ --c-hpath lcmtypes/ $^

LCMTYPES_SOURCE = lcmtypes/settings_t.c lcmtypes/reset_t.c lcmtypes/world_t.c lcmtypes/agent_t.c lcmtypes/action_t.c

# Now instead of listing all the LCM type files individually, we can just add $(LCMTYPES_SOURCE) to any list of .c files

gui: gui.c bmp.c image_server.c graphics.c vector_xy_t.c $(LCMTYPES_SOURCE)
	gcc -o $@ $^ $(CFLAGS) -std=c11 -lm -lpthread -llcm -O0

#gui: gui.c bmp.c image_server.c graphics.c vector_xy_t.c collision.c
#	gcc -o $@ $^ $(CFLAGS) -std=c11 -lm -lpthread -O0

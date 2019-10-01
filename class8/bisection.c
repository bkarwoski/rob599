#include <stdlib.h>
#include <math.h>

int main(int argc, char **argv) {
        if (argc != 4) {
        fprintf(stderr, "usage: %s <file> <key> <lines before>\n", argv[0]);
        return 1;
    }
    return 0;
}
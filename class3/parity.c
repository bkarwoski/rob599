#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("0\n");
        return 0;
    }
    if (argc > 2) {
        printf("usage: ./parity [<text>]\n");
        return 1;
    }

return 0;
}


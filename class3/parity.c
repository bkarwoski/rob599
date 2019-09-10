#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("0\n");
        return 0;
    }
    if (argc > 2) {
        printf("usage: ./parity [<text>]\n");
        return 1;
    }
    int counter = 0;
    for (int j = 0; j < strlen(argv[1]); j++) {
        //printf("strlen(argv[1]) = %d\n", strlen(argv[1]));
        //printf("%c\n", argv[1][j]);
        //printf("%d\n", argv[1][j] & 1);
        char let = argv[1][j];
        while (let > 0) {
            if (let & 1) {
                counter++;
            }
            let = let >> 1;
        }
    }
    //printf("counter = %d\n", counter);
    int parity = counter % 2;
    printf("%d\n", parity);

    return 0;
}


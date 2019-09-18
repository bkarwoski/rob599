#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc == 1) {
        fprintf(stderr, "usage: ./cryptogram <encrypt|decrypt> <password> <text>\n");
        return 1;
    }
    if (strcmp(argv[1], "encrypt") == 0) {

    } else if (strcmp(argv[1], "decrypt") == 0) {

    } else {
        fprintf(stderr, "expected command encrypt or decrypt.\n");
        return 1;
    }
    char fpass[100];
    int j = 0;
    for (int i = 0; argv[3][i] != '\0'; i++) {
        if ((argv[3][i] >= 'a') && (argv[3][i] <= 'z')) {
            fpass[j] = argv[3][i];
            j++;
        }
        if ((argv[3][i] >= 'A') && (argv[3][i] <= 'Z')) {
            fpass[j] = (argv[3][i] - 'A' + 'a');
            j++;
        }
    }


}

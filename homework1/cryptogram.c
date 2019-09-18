#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
    if (argc == 1) {
        fprintf(stderr, "usage: ./cryptogram <encrypt|decrypt> <password> <text>\n");
        return 1;
    }
    bool encrpyt;
    if (strcmp(argv[1], "encrypt") == 0) {
        encrpyt = 1;
    } else if (strcmp(argv[1], "decrypt") == 0) {
        encrpyt = 0;
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
    int pass_len = j;
    if (pass_len == 0) {
        fpass[0] = 'a';
        pass_len = 1;
    }
    if (encrpyt) {
        for (int i = 0; argv[2][i] != '\0'; i++) {
            if ((argv[2][i] >= 'a') && (argv[2][i] <= 'z')) {
                argv[2][i] += (fpass[i % pass_len] - 'a');
                if (argv[2][i] > 'z') {
                    argv[2][i] -= 26;
                }

            }
            if ((argv[2][i] >= 'A') && (argv[2][i] <= 'Z')) {
                argv[2][i] += (fpass[i % pass_len] - 'a');
                if (argv[2][i] > 'Z') {
                    argv[2][i] -= 26;
                }

            }
        }
    } else {
        //decrypt
    }
    printf("%s\n", argv[2]);
    return 0;
}

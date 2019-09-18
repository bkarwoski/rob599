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
    char *opass = argv[2];
    char fpass[100];
    int j = 0;
    for (int i = 0; opass[i] != '\0'; i++) {
        if ((opass[i] >= 'a') && (opass[i] <= 'z')) {
            fpass[j] = opass[i];
            j++;
        }
        if ((opass[i] >= 'A') && (opass[i] <= 'Z')) {
            fpass[j] = (opass[i] - 'A' + 'a');
            j++;
        }
    }
    int pass_len = j;
    if (pass_len == 0) {
        fpass[0] = 'a';
        pass_len = 1;
    }
    char *text = argv[3];
    // printf("    orig password is %s\n", opass);
    // printf("filtered password is %s\n", fpass);
    // printf("orig text is %s\n", text);
    if (encrpyt) {
        for (int i = 0; text[i] != '\0'; i++) {
            if ((text[i] >= 'a') && (text[i] <= 'z')) {
                text[i] += (fpass[i % pass_len] - 'a');
                if (text[i] > 'z') {
                    text[i] -= 26;
                }
            }
            if ((text[i] >= 'A') && (text[i] <= 'Z')) {
                text[i] += (fpass[i % pass_len] - 'a');
                if (text[i] > 'Z') {
                    text[i] -= 26;
                }
            }
        }
    } else {
        for (int i = 0; text[i] != '\0'; i++) {
            if ((text[i] >= 'a') && (text[i] <= 'z')) {
                text[i] -= (fpass[i % pass_len] - 'a');
                if (text[i] < 'a') {
                    text[i] += 26;
                }
            }
            if ((text[i] >= 'A') && (text[i] <= 'Z')) {
                text[i] -= (fpass[i % pass_len] - 'a');
                if (text[i] < 'A') {
                    text[i] += 26;
                }
            }
        }
    }
    printf("%s\n", text);
    return 0;
}

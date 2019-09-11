#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

char *trit_encodings[243];

int main(void) {
    for (int i = 0; i < 243; i++) {
        trit_encodings[i] = calloc(sizeof(char), 6);
    }

    trit_encodings[0][0] = ' ';
    trit_encodings[0][1] = ' ';
    trit_encodings[0][2] = ' ';
    trit_encodings[0][3] = ' ';
    trit_encodings[0][4] = ' ';

    for (int i = 1; i < 243; i++) {
        strncpy(trit_encodings[i], trit_encodings[i-1], 6);
        for (int j = 0; j <= 4; j++) {
            if (trit_encodings[i][j] == ' ') {
                trit_encodings[i][j] = ':';
                break;
            }
            if (trit_encodings[i][j] == ':') {
                trit_encodings[i][j] = '@';
                break;
            }
            if (trit_encodings[i][j] == '@') {
                trit_encodings[i][j] = ' ';
            }
        }
        //printf("trit[%d]: %s\n", i, trit_encodings[i]);
    }

    FILE *img;
    img = fopen("img.bin", "rb");

    for(int i = 0; i < 40; i++) {
        for (int j = 0; j < 18; j++) {
            char c = fgetc(img);
            printf("%s", trit_encodings[(uint8_t)c]);
        }
        printf("\n");
    }

    return 0;
}


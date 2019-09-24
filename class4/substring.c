#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

char file_buffer[64*1024];

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "usage: %s <file> <key> <lines before>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        fprintf(stderr, "Could not open %s: ", argv[1]);
        perror("");
        return 1;
    }
    int bytes_read = fread(file_buffer, 1, sizeof(file_buffer) - 1, f);
    if (!feof(f)) {
        fprintf(stderr, "Could not read entire file. Is it too big?\n");
        return 1;
    }
    int lineCount = atoi(argv[0]);
    char* key = argv[2];
    int keyLength = strlen(key);
    int pos = 0;
    char *output = '\0';

    while (true) {
        if (lineCount == 0) {
            output = strstr(&file_buffer[pos], key);
            pos = output - file_buffer + 1;
            //printf("pos: %d\n", pos);
            if (output == NULL) {
                break;
            }
            char tmp = output[keyLength];
            output[keyLength] = '\0';
            printf("%s\n\n", output);
            output[keyLength] = tmp;
        } else {
        }
    }

    fclose(f);
    // we want this to be a null-treminated string,
    // but fread just reads the file as binary, so we add it ourselves
    file_buffer[bytes_read] = '\0';

    return 0;
}

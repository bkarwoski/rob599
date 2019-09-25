#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

char file_buffer[64*1024];

char* getContext(int numLines, char* buf, char* pos) {
    char *tmp = pos;
    int index = pos - buf;
    printf("init numLines == %d\n", numLines);
    while(numLines != 0) {
        //printf("numLines == %d\n", numLines);
        if (index == 0) {
            break;
        }
        if (buf[index] == '\n') {
            numLines--;
        }
        //printf("decrementing!\n");
        tmp--;
        index--;
    }
    return tmp;
}

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
    int lineCount = atoi(argv[3]);
    char* key = argv[2];
    int keyLength = strlen(key);
    int index = 0;

    while (true) {
        char* pos = strstr(&file_buffer[index], key);
        if (!pos) {
            break;
        }
        index = pos - file_buffer + 1;
        //printf("index: %d\n", index);
        char* output = getContext(lineCount, file_buffer, pos);
        char tmp = output[keyLength];
        pos[keyLength] = '\0';
        printf("%s\n\n", output);
        pos[keyLength] = tmp;
    }

    fclose(f);
    // we want this to be a null-treminated string,
    // but fread just reads the file as binary, so we add it ourselves
    file_buffer[bytes_read] = '\0';

    return 0;
}

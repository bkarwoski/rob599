#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
    printf("sizeof(key) == %ld\n", sizeof(key));
    printf("sizeof(key[0]) == %ld\n",sizeof(key[0]));
    printf("sizeof(argv[2]) == %ld\n",sizeof(argv[2]));
    int keyLength = (sizeof(key) / sizeof(key[0]));

    if (lineCount == 0) {
     //   fgets()
     //   if((strstr(temp, key)) != NULL) {
        char *output = strstr(file_buffer, key);
        printf("%s\n", output);
        printf("key: %s\n", key);
        printf("key size: %d\n", keyLength);
    }

    fclose(f);
    // we want this to be a null-treminated string,
    // but fread just reads the file as binary, so we add it ourselves
    file_buffer[bytes_read] = '\0';

    return 0;
}

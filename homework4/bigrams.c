#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "hashtable.h"
#define MAX_WORD_LENGTH 256
#define MAX_KEY_SIZE 256

bool read_word(FILE *f, char *word, int wordLength) {
    bool isEOF = false;
    char next = ' ';
    int word_i = 0;
    while (true) {
        next = (char)fgetc(f);
        if (next == EOF) {
            //printf("EOF\n");
            //isEOF = true;
            return true;
        }
        if ((next >= 'a' && next <= 'z') || (next >= 'A' && next <= 'Z')) {
            break;
        }
    }
    while (((next >= 'a' && next <= 'z') || (next >= 'A' && next <= 'Z')) &&
           word_i < wordLength) {
        word[word_i] = next;
        word_i++;
        next = (char)fgetc(f);
    }
    word[word_i] = '\0';
    //return isEOF;
    return false;
}

int main(int argc, char *argv[]) {
    char *file = "book.txt";
    if (argc > 1) {
        file = argv[1];
    }

    FILE *f = fopen(file, "r");
    if (!f) {
        fprintf(stderr, "Failed to open %s: ", file);
        perror("");
        return 1;
    }
    hashtable_t *ht = hashtable_create(128);
    char *word = malloc(MAX_WORD_LENGTH);
    char *nextWord = malloc(MAX_WORD_LENGTH);
    char *tmp;
    char *bigram = malloc(2 * MAX_WORD_LENGTH);
    read_word(f, word, MAX_WORD_LENGTH);
    while (true) {
        if (read_word(f, nextWord, MAX_WORD_LENGTH)) {
            break;
        }

        snprintf(bigram, 2 * MAX_WORD_LENGTH, "%s %s", word, nextWord);
        //printf("%s\n", bigram);
        int bigramCount = 0;
        hashtable_get(ht, bigram, &bigramCount);
        bigramCount++;
        hashtable_set(ht, bigram, bigramCount);
        tmp = word;
        word = nextWord;
        nextWord = tmp;
    }
    bool exceeds200 = false;
    for (int i = 0; i < hashtable_probe_max(ht); i++) {
        char *key = " ";
        int val = 0;
        hashtable_probe(ht, i, &key, &val);
        if (val >= 200) {
            printf("Bigram '%s' has count of %d\n", key, val);
            exceeds200 = true;
        }
    }
    if (!exceeds200) {
        for (int i = 0; i < hashtable_probe_max(ht); i++) {
            char *key = " ";
            int val = 0;
            if (hashtable_probe(ht, i, &key, &val)) {
                printf("Bigram '%s' has count of %d\n", key, val);
            }
        }
    }
    printf("Total of %d different bigrams recorded\n", hashtable_size(ht));
    free(word);
    free(nextWord);
    free(bigram);
    hashtable_destroy(ht);
    return 0;
}

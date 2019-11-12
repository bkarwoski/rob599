#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "hashtable.h"
#define MAX_WORD_LENGTH 256

bool read_word(FILE *f, char *word, int wordLength) {
    bool isEOF = false;
    char next = ' ';
    int word_i = 0;
    while(true) {
        next = fgetc(f);
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
        next = fgetc(f);
    }
    word[word_i] = '\0';
    //return isEOF;
    return false;
}

int main(int argc, char* argv[]) {
    hashtable_t *ht = hashtable_create(128);
    char *key = "apple";
    int value = 100;
    hashtable_set(ht, key, value);
    value = 99;
    hashtable_get(ht, key, &value);
    printf("%d\n", value);
    hashtable_destroy(ht);
    FILE *f = fopen("test_book.txt", "r");
    if (!f) {
        perror("Failed to open words.txt");
        return 1;
    }
    char *word = malloc(MAX_WORD_LENGTH);
    char *nextWord = malloc(MAX_WORD_LENGTH);
    char *tmp;
    char *bigram = malloc(2 * MAX_WORD_LENGTH);
    read_word(f, word, MAX_WORD_LENGTH);
    while (true) {
        if (read_word(f, nextWord, MAX_WORD_LENGTH)) {
            break;
        }
        snprintf(bigram, 2*MAX_WORD_LENGTH, "%s %s", word, nextWord);
        printf("%s\n", bigram);
        tmp = word;
        word = nextWord;
        nextWord = tmp;
    }
    free(word);
    free(nextWord);
    free(bigram);
    return 0;
}
